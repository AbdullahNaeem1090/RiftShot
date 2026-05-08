// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RS_CombatComponent.generated.h"

class ARS_PlayerController;
class ARS_HUD;
class ARS_BaseWeapon;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RIFTSHOT_API URS_CombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	URS_CombatComponent();
	friend class ARS_Character;
	
	void FireButtonPressed(bool bPresses);
	FORCEINLINE FVector GetTargetLocation() const { return HitTargetLocation; };
	
	
protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual  void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(server, reliable)
	void ServerFire(const FVector_NetQuantize& TargetLocation);
	
	UFUNCTION(NetMulticast,reliable)
	void MulticastFire(const FVector_NetQuantize& TargetLocation);
	
	
	void PerformLineTrace();
	
private:
	void SetAiming(bool bValue);
	
	void SetHudCrossHairs(float DeltaTime);
	
	UFUNCTION(Server, Reliable)
	void ServerSetAiming(bool bValue);
	
	UFUNCTION()
	void OnRep_EquippedWeapon();
	
	void Equip(ARS_BaseWeapon* InWeapon);
	
	UPROPERTY()
	TObjectPtr<ARS_Character> Character;
	
	UPROPERTY()
	TObjectPtr<ARS_HUD> HUD;
	
	UPROPERTY()
	TObjectPtr<ARS_PlayerController> Controller;
	
	UPROPERTY(ReplicatedUsing=OnRep_EquippedWeapon)
	TObjectPtr<ARS_BaseWeapon> EquippedWeapon;
	
	UPROPERTY(Replicated)
	bool bIsAiming=false;
	
	bool bFireButtonPressed;
	
	FHitResult TraceHitResult;
	
	FVector HitTargetLocation;
	
	float DefaultFOV;
	float CurrentFOV;
	void  UpdateFOV(float DeltaTime);
	void Fire();

	FTimerHandle FireTimer;
	bool bCanFire = true;

	void StartFireTimer();
	void FireTimerFinished();
	
};

