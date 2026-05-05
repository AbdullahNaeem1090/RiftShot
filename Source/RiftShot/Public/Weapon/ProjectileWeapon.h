// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RS_BaseWeapon.h"
#include "ProjectileWeapon.generated.h"

UCLASS()
class RIFTSHOT_API AProjectileWeapon : public ARS_BaseWeapon
{
	GENERATED_BODY()

public:
	AProjectileWeapon();
	virtual void Fire(FVector TargetLocation) override;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
