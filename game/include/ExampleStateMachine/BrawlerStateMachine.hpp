#pragma once

#include <Canis/Entity.hpp>

#include <SuperPupUtilities/StateMachine.hpp>

#include <string>

namespace ExampleStateMachine
{
    class BrawlerStateMachine;

    class IdleState : public SuperPupUtilities::State
    {
    public:
        static constexpr const char* Name = "IdleState";

        explicit IdleState(BrawlerStateMachine& _owner);
        void Enter() override;
        void Update(float _dt) override;

    private:
        BrawlerStateMachine& m_owner;
    };

    class ChaseState : public SuperPupUtilities::State
    {
    public:
        static constexpr const char* Name = "ChaseState";

        explicit ChaseState(BrawlerStateMachine& _owner);
        void Enter() override;
        void Update(float _dt) override;

    private:
        BrawlerStateMachine& m_owner;
    };

    class HammerTimeState : public SuperPupUtilities::State
    {
    public:
        static constexpr const char* Name = "HammerTimeState";

        explicit HammerTimeState(BrawlerStateMachine& _owner);
        void Enter() override;
        void Update(float _dt) override;
        void Exit() override;

    private:
        BrawlerStateMachine& m_owner;
    };

    class BrawlerStateMachine : public Canis::ScriptableEntity
    {
    public:
        static constexpr const char* ScriptName = "ExampleStateMachine::BrawlerStateMachine";

        std::string targetTag = "";
        float detectionRange = 20.0f;
        float moveSpeed = 4.0f;
        float attackRange = 2.25f;
        float attackDuration = 0.75f;
        float attackDamageTime = 0.25f;
        int attackDamage = 10;
        int maxHealth = 40;
        bool logStateChanges = true;
        Canis::Entity* hammerVisual = nullptr;
        Canis::AudioAssetHandle hitSFX = {};
        float hitVolume = 0.35f;
        Canis::AudioAssetHandle defeatSFX = {};
        float defeatVolume = 0.55f;

        explicit BrawlerStateMachine(Canis::Entity& _entity);

        void Create() override;
        void Ready() override;
        void Destroy() override;
        void Update(float _dt) override;

        Canis::Entity* FindClosestTarget() const;
        float DistanceTo(const Canis::Entity& _other) const;
        void FaceTarget(const Canis::Entity& _target);
        void MoveTowards(const Canis::Entity& _target, float _dt);
        void ChangeState(const std::string& _stateName);
        const std::string& GetCurrentStateName() const;
        int GetCurrentHealth() const;

        void ResetHammerPose();
        void SetHammerSwing(float _normalized);
        void ApplyAttackDamage();
        void TakeDamage(int _damage);
        bool IsAlive() const;

    private:
        friend class IdleState;
        friend class ChaseState;
        friend class HammerTimeState;

        SuperPupUtilities::StateMachine m_stateMachine = {};
        IdleState m_idleState;
        ChaseState m_chaseState;
        HammerTimeState m_hammerTimeState;

        int m_currentHealth = 0;
        float m_stateTime = 0.0f;
        bool m_attackDamageApplied = false;
        Canis::Vector4 m_baseColor = Canis::Vector4(1.0f);
        bool m_hasBaseColor = false;
    };

    void RegisterBrawlerStateMachineScript(Canis::App& _app);
    void UnRegisterBrawlerStateMachineScript(Canis::App& _app);
}
