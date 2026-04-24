// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RS_CombatComponent.generated.h"

class ARS_BaseWeapon;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RIFTSHOT_API URS_CombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	URS_CombatComponent();
	friend class ARS_Character;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
protected:
	virtual void BeginPlay() override;
	
private:
	void Equip(ARS_BaseWeapon* InWeapon);
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ARS_Character> Character;
	
	UPROPERTY(Replicated,VisibleAnywhere)
	TObjectPtr<ARS_BaseWeapon> EquippedWeapon;
	
};
