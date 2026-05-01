#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RS_Character.generated.h"

/* ============================= */
// Forward Declarations          

class URS_CombatComponent;
class ARS_BaseWeapon;
class UWidgetComponent;
class USpringArmComponent;
class UCameraComponent;
class UAnimInstance;
class UAnimMontage;

/* ============================= */
// Enums                         

UENUM(BlueprintType)
enum class ERS_RotationMode : uint8
{
    OrientToMovement,
    Strafe
};

UENUM(BlueprintType)
enum class ERS_Gait : uint8
{
    Walk,
    Jog,
    Crouch
};

/* ============================= */
// RS_Character Class               

UCLASS()
class RIFTSHOT_API ARS_Character : public ACharacter
{
    GENERATED_BODY()

public:
    
    void PlayFireMontage(bool bIsAiming);

    /* ============================= */
    // Constructor & Overrides       

    ARS_Character();

    virtual void Tick(float DeltaTime) override;
    virtual void BeginPlay() override;
    virtual void PostInitializeComponents() override;

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    virtual void OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;
    virtual void OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;

    /* ============================= */
    // Core Gameplay API             

    void SetOverlappingWeapon(ARS_BaseWeapon* InWeapon);
    void SetRotationMode(ERS_RotationMode Mode);
    void SetNewAnimLayer();

    void EquipPressed();
    void ToggleCrouch();

    void StartAiming();
    void StopAiming();
    
    void FireStart(); 
    void FireStop();

    /* ============================= */
    // Getters                      */

    bool IsWeaponEquipped() const;
    bool IsAiming() const;

    /* ============================= */
    // Replication Variables (Public Read)

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Combat")
    float AimPitch = 0.f;

    UPROPERTY(BlueprintReadOnly, Category = "Movement")
    float DistanceToGround = 0.f;

    UPROPERTY(BlueprintReadOnly, Category = "Movement")
    ERS_RotationMode RotationMode = ERS_RotationMode::OrientToMovement;

protected:

    /* ============================= */
    // Components                    

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<USpringArmComponent> SpringArmComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UCameraComponent> CameraComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UWidgetComponent> OverHeadWidgetComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<URS_CombatComponent> CombatComponent;

    /* ============================= */
    // Animation         

    UPROPERTY(ReplicatedUsing = OnRep_AnimLayerChanged)
    TSubclassOf<UAnimInstance> CurrentLayer;

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    TSubclassOf<UAnimInstance> UnArmedLayer;

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    TSubclassOf<UAnimInstance> RifleLayer;
    
    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    TObjectPtr<UAnimMontage> FireMontage;

    /* ============================= */
    // Weapon                        

    UPROPERTY(ReplicatedUsing = OnRep_OverlappingWeapon)
    TObjectPtr<ARS_BaseWeapon> OverlappingWeapon;

    /* ============================= */
    // Movement State                

    UPROPERTY(ReplicatedUsing = OnRep_GaitChanged)
    ERS_Gait CurrentGait = ERS_Gait::Jog;

    UPROPERTY(Replicated)
    ERS_Gait PreviousGait = ERS_Gait::Jog;

protected:
    /* ============================= */
    // Server RPCs

    UFUNCTION(Server, Reliable)
    void ServerEquipPressed();

    UFUNCTION(Server, Unreliable)
    void Server_SetAimPitch(float NewPitch);
    
    /* ============================= */
    // Replication Callbacks 

    UFUNCTION()
    void OnRep_OverlappingWeapon(ARS_BaseWeapon* LastWeapon);

    UFUNCTION()
    void OnRep_AnimLayerChanged();

    UFUNCTION()
    void OnRep_GaitChanged();

    /* ============================= */
    // Internal Logic

    void ApplyGaitToAnimInstance();

    UFUNCTION()
    void SetGait(ERS_Gait NewGait);

    float GetGroundDistance() const;
    

};