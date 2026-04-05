#include <ExampleStateMachine/BrawlerStateMachine.hpp>

#include <Canis/App.hpp>
#include <Canis/AudioManager.hpp>
#include <Canis/ConfigHelper.hpp>
#include <Canis/Debug.hpp>

#include <algorithm>
#include <cmath>
#include <limits>

namespace ExampleStateMachine
{
    namespace
    {
        ScriptConf brawlerStateMachineConf = {};

        constexpr float HammerRestDegrees = -50.0f;
        constexpr float HammerSwingDegrees = 140.0f;
        constexpr float HammerReachPadding = 0.35f;

        float Clamp01(float _value)
        {
            return std::clamp(_value, 0.0f, 1.0f);
        }

        float DegreesToRadians(float _degrees)
        {
            return _degrees * Canis::DEG2RAD;
        }
    }

    IdleState::IdleState(BrawlerStateMachine& _owner) :
        State(Name),
        m_owner(_owner) {}

    void IdleState::Enter()
    {
        m_owner.ResetHammerPose();
    }

    void IdleState::Update(float)
    {
        if (m_owner.FindClosestTarget() != nullptr)
            m_owner.ChangeState(ChaseState::Name);
    }

    ChaseState::ChaseState(BrawlerStateMachine& _owner) :
        State(Name),
        m_owner(_owner) {}

    void ChaseState::Enter()
    {
        m_owner.ResetHammerPose();
    }

    void ChaseState::Update(float _dt)
    {
        Canis::Entity* target = m_owner.FindClosestTarget();

        if (target == nullptr)
        {
            m_owner.ChangeState(IdleState::Name);
            return;
        }

        m_owner.FaceTarget(*target);

        if (m_owner.DistanceTo(*target) <= m_owner.attackRange)
        {
            m_owner.ChangeState(HammerTimeState::Name);
            return;
        }

        m_owner.MoveTowards(*target, _dt);
    }

    HammerTimeState::HammerTimeState(BrawlerStateMachine& _owner) :
        State(Name),
        m_owner(_owner) {}

    void HammerTimeState::Enter()
    {
        m_owner.m_attackDamageApplied = false;
        m_owner.SetHammerSwing(0.0f);
    }

    void HammerTimeState::Update(float)
    {
        if (Canis::Entity* target = m_owner.FindClosestTarget())
            m_owner.FaceTarget(*target);

        const float duration = std::max(m_owner.attackDuration, 0.001f);
        m_owner.SetHammerSwing(m_owner.m_stateTime / duration);

        if (!m_owner.m_attackDamageApplied && m_owner.m_stateTime >= m_owner.attackDamageTime)
        {
            m_owner.m_attackDamageApplied = true;
            m_owner.ApplyAttackDamage();
        }

        if (m_owner.m_stateTime < duration)
            return;

        if (m_owner.FindClosestTarget() != nullptr)
            m_owner.ChangeState(ChaseState::Name);
        else
            m_owner.ChangeState(IdleState::Name);
    }

    void HammerTimeState::Exit()
    {
        m_owner.ResetHammerPose();
    }

    BrawlerStateMachine::BrawlerStateMachine(Canis::Entity& _entity) :
        Canis::ScriptableEntity(_entity),
        m_idleState(*this),
        m_chaseState(*this),
        m_hammerTimeState(*this) {}

    void RegisterBrawlerStateMachineScript(Canis::App& _app)
    {
        REGISTER_PROPERTY(brawlerStateMachineConf, ExampleStateMachine::BrawlerStateMachine, targetTag);
        REGISTER_PROPERTY(brawlerStateMachineConf, ExampleStateMachine::BrawlerStateMachine, detectionRange);
        REGISTER_PROPERTY(brawlerStateMachineConf, ExampleStateMachine::BrawlerStateMachine, moveSpeed);
        REGISTER_PROPERTY(brawlerStateMachineConf, ExampleStateMachine::BrawlerStateMachine, attackRange);
        REGISTER_PROPERTY(brawlerStateMachineConf, ExampleStateMachine::BrawlerStateMachine, attackDuration);
        REGISTER_PROPERTY(brawlerStateMachineConf, ExampleStateMachine::BrawlerStateMachine, attackDamageTime);
        REGISTER_PROPERTY(brawlerStateMachineConf, ExampleStateMachine::BrawlerStateMachine, attackDamage);
        REGISTER_PROPERTY(brawlerStateMachineConf, ExampleStateMachine::BrawlerStateMachine, maxHealth);
        REGISTER_PROPERTY(brawlerStateMachineConf, ExampleStateMachine::BrawlerStateMachine, logStateChanges);
        REGISTER_PROPERTY(brawlerStateMachineConf, ExampleStateMachine::BrawlerStateMachine, hammerVisual);
        REGISTER_PROPERTY(brawlerStateMachineConf, ExampleStateMachine::BrawlerStateMachine, hitSFX);
        REGISTER_PROPERTY(brawlerStateMachineConf, ExampleStateMachine::BrawlerStateMachine, hitVolume);
        REGISTER_PROPERTY(brawlerStateMachineConf, ExampleStateMachine::BrawlerStateMachine, defeatSFX);
        REGISTER_PROPERTY(brawlerStateMachineConf, ExampleStateMachine::BrawlerStateMachine, defeatVolume);

        DEFAULT_CONFIG_AND_REQUIRED(
            brawlerStateMachineConf,
            ExampleStateMachine::BrawlerStateMachine,
            Canis::Transform,
            Canis::Material,
            Canis::Model);

        brawlerStateMachineConf.DEFAULT_DRAW_INSPECTOR(
            ExampleStateMachine::BrawlerStateMachine,
            ImGui::Text("Current State: %s", component->GetCurrentStateName().c_str());
            ImGui::Text("Health: %d / %d", component->GetCurrentHealth(), component->maxHealth);
        );

        _app.RegisterScript(brawlerStateMachineConf);
    }

    DEFAULT_UNREGISTER_SCRIPT(brawlerStateMachineConf, BrawlerStateMachine)

    void BrawlerStateMachine::Create()
    {
        if (entity.HasComponent<Canis::Material>())
        {
            m_baseColor = entity.GetComponent<Canis::Material>().color;
            m_hasBaseColor = true;
        }
    }

    void BrawlerStateMachine::Ready()
    {
        if (entity.HasComponent<Canis::Material>())
        {
            m_baseColor = entity.GetComponent<Canis::Material>().color;
            m_hasBaseColor = true;
        }

        m_currentHealth = std::max(maxHealth, 1);
        m_stateTime = 0.0f;
        m_attackDamageApplied = false;

        m_stateMachine.ClearStates();
        m_stateMachine.AddState(m_idleState);
        m_stateMachine.AddState(m_chaseState);
        m_stateMachine.AddState(m_hammerTimeState);

        ResetHammerPose();
        ChangeState(IdleState::Name);
    }

    void BrawlerStateMachine::Destroy()
    {
        hammerVisual = nullptr;
        m_stateMachine.ClearStates();
    }

    void BrawlerStateMachine::Update(float _dt)
    {
        if (!IsAlive())
            return;

        m_stateTime += _dt;
        m_stateMachine.Update(_dt);
    }

    Canis::Entity* BrawlerStateMachine::FindClosestTarget() const
    {
        if (targetTag.empty() || !entity.HasComponent<Canis::Transform>())
            return nullptr;

        const Canis::Transform& transform = entity.GetComponent<Canis::Transform>();
        const Canis::Vector3 origin = transform.GetGlobalPosition();
        Canis::Entity* closestTarget = nullptr;
        float closestDistance = detectionRange;

        for (Canis::Entity* candidate : entity.scene.GetEntitiesWithTag(targetTag))
        {
            if (candidate == nullptr || candidate == &entity || !candidate->active)
                continue;

            if (!candidate->HasComponent<Canis::Transform>())
                continue;

            if (const BrawlerStateMachine* other = candidate->GetScript<BrawlerStateMachine>())
            {
                if (!other->IsAlive())
                    continue;
            }

            const Canis::Vector3 candidatePosition = candidate->GetComponent<Canis::Transform>().GetGlobalPosition();
            const float distance = glm::distance(origin, candidatePosition);

            if (distance > detectionRange || distance >= closestDistance)
                continue;

            closestDistance = distance;
            closestTarget = candidate;
        }

        return closestTarget;
    }

    float BrawlerStateMachine::DistanceTo(const Canis::Entity& _other) const
    {
        if (!entity.HasComponent<Canis::Transform>() || !_other.HasComponent<Canis::Transform>())
            return std::numeric_limits<float>::max();

        const Canis::Vector3 selfPosition = entity.GetComponent<Canis::Transform>().GetGlobalPosition();
        const Canis::Vector3 targetPosition = _other.GetComponent<Canis::Transform>().GetGlobalPosition();
        return glm::distance(selfPosition, targetPosition);
    }

    void BrawlerStateMachine::FaceTarget(const Canis::Entity& _target)
    {
        if (!entity.HasComponent<Canis::Transform>() || !_target.HasComponent<Canis::Transform>())
            return;

        Canis::Transform& transform = entity.GetComponent<Canis::Transform>();
        const Canis::Vector3 selfPosition = transform.GetGlobalPosition();
        Canis::Vector3 direction = _target.GetComponent<Canis::Transform>().GetGlobalPosition() - selfPosition;
        direction.y = 0.0f;

        if (glm::dot(direction, direction) <= 0.0001f)
            return;

        direction = glm::normalize(direction);
        transform.rotation.y = std::atan2(-direction.x, -direction.z);
    }

    void BrawlerStateMachine::MoveTowards(const Canis::Entity& _target, float _dt)
    {
        if (!entity.HasComponent<Canis::Transform>() || !_target.HasComponent<Canis::Transform>())
            return;

        Canis::Transform& transform = entity.GetComponent<Canis::Transform>();
        const Canis::Vector3 selfPosition = transform.GetGlobalPosition();
        Canis::Vector3 direction = _target.GetComponent<Canis::Transform>().GetGlobalPosition() - selfPosition;
        direction.y = 0.0f;

        if (glm::dot(direction, direction) <= 0.0001f)
            return;

        direction = glm::normalize(direction);
        transform.position += direction * moveSpeed * _dt;
    }

    void BrawlerStateMachine::ChangeState(const std::string& _stateName)
    {
        if (m_stateMachine.GetCurrentStateName() == _stateName)
            return;

        if (!m_stateMachine.ChangeState(_stateName))
            return;

        m_stateTime = 0.0f;

        if (logStateChanges)
            Canis::Debug::Log("%s -> %s", entity.name.c_str(), _stateName.c_str());
    }

    const std::string& BrawlerStateMachine::GetCurrentStateName() const
    {
        return m_stateMachine.GetCurrentStateName();
    }

    int BrawlerStateMachine::GetCurrentHealth() const
    {
        return m_currentHealth;
    }

    void BrawlerStateMachine::ResetHammerPose()
    {
        SetHammerSwing(0.0f);
    }

    void BrawlerStateMachine::SetHammerSwing(float _normalized)
    {
        if (hammerVisual == nullptr || !hammerVisual->HasComponent<Canis::Transform>())
            return;

        Canis::Transform& hammerTransform = hammerVisual->GetComponent<Canis::Transform>();
        const float normalized = Clamp01(_normalized);
        const float swingBlend = (normalized <= 0.5f)
            ? normalized * 2.0f
            : (1.0f - normalized) * 2.0f;

        hammerTransform.rotation.z = DegreesToRadians(HammerRestDegrees + (HammerSwingDegrees * swingBlend));
    }

    void BrawlerStateMachine::ApplyAttackDamage()
    {
        Canis::Entity* target = FindClosestTarget();

        if (target == nullptr)
            return;

        if (DistanceTo(*target) > (attackRange + HammerReachPadding))
            return;

        if (BrawlerStateMachine* other = target->GetScript<BrawlerStateMachine>())
        {
            other->TakeDamage(attackDamage);
            Canis::AudioManager::PlaySFX(hitSFX, hitVolume);
        }
    }

    void BrawlerStateMachine::TakeDamage(int _damage)
    {
        if (!IsAlive())
            return;

        m_currentHealth = std::max(0, m_currentHealth - std::max(_damage, 0));

        if (m_hasBaseColor && entity.HasComponent<Canis::Material>())
        {
            Canis::Material& material = entity.GetComponent<Canis::Material>();
            const float healthRatio = (maxHealth > 0)
                ? (static_cast<float>(m_currentHealth) / static_cast<float>(maxHealth))
                : 0.0f;

            material.color = Canis::Vector4(
                m_baseColor.x * (0.5f + (0.5f * healthRatio)),
                m_baseColor.y * (0.5f + (0.5f * healthRatio)),
                m_baseColor.z * (0.5f + (0.5f * healthRatio)),
                m_baseColor.w);
        }

        if (m_currentHealth > 0)
            return;

        if (logStateChanges)
            Canis::Debug::Log("%s was defeated.", entity.name.c_str());

        Canis::AudioManager::PlaySFX(defeatSFX, defeatVolume);
        entity.Destroy();
    }

    bool BrawlerStateMachine::IsAlive() const
    {
        return m_currentHealth > 0;
    }
}
