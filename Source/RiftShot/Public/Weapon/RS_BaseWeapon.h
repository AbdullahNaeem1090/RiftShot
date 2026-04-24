// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RS_BaseWeapon.generated.h"

class UWidgetComponent;
class USphereComponent;


UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	Ews_Initial UMETA(DisplayName="Initial"),
	Ews_Equipped UMETA(DisplayName="Equipped"),
	Ews_Dropped UMETA(DisplayName="Dropped"),

	Ews_MAX UMETA(Hidden),
};

UCLASS()
class RIFTSHOT_API ARS_BaseWeapon : public AActor
{
	GENERATED_BODY()

public:
	ARS_BaseWeapon();
	void SetPickUpWidgetVisibility(bool Visible);
	
protected:
	virtual void BeginPlay() override;
	UFUNCTION()
	virtual void OnSphereOverlap( UPrimitiveComponent* OverlappedComponent,AActor* OtherActor,UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,bool bFromSweep,const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor,UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex);
	
private:

protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<USphereComponent> SphereComponent;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> PickUpWidget;
	
private:
	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	EWeaponState WeaponState;
};
