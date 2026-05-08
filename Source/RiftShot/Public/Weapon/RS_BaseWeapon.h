// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RS_BaseWeapon.generated.h"

class AShellCasing;
class AProjectile;
class UWidgetComponent;
class USphereComponent;
class UAnimationAsset;
class UTexture2D;


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
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	void SetPickUpWidgetVisibility(bool Visible);
	void SetWeaponState(EWeaponState NewState);
	virtual void Fire(FVector TargetLocation);
	USkeletalMeshComponent* GetWeaponMesh(){return SkeletalMeshComponent;}
	
protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	virtual void OnSphereOverlap( UPrimitiveComponent* OverlappedComponent,AActor* OtherActor,UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,bool bFromSweep,const FHitResult& SweepResult);
	
	UFUNCTION()
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor,UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex);
	
	UFUNCTION()
	void OnRep_WeaponState();
	
public:
	UPROPERTY(EditAnywhere, Category = Crosshairs)
	TObjectPtr<UTexture2D> CrosshairsCenter;
	
	UPROPERTY(EditAnywhere, Category = Crosshairs)
	TObjectPtr<UTexture2D> CrosshairsLeft;
	
	UPROPERTY(EditAnywhere, Category = Crosshairs)
	TObjectPtr<UTexture2D> CrosshairsRight;
	
	UPROPERTY(EditAnywhere, Category = Crosshairs)
	TObjectPtr<UTexture2D> CrosshairsTop;
	
	UPROPERTY(EditAnywhere, Category = Crosshairs)
	TObjectPtr<UTexture2D> CrosshairsBottom;
	
	UPROPERTY(EditAnywhere)
	float ZoomFOV=30.f;
	
	UPROPERTY(EditAnywhere)
	float ZoomInterpSpeed=10;
	
	
	UPROPERTY(EditAnywhere, Category = Combat)
	float FireDelay = .15f;

	UPROPERTY(EditAnywhere, Category = Combat)
	bool bAutomatic = true;

protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<USphereComponent> SphereComponent;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> PickUpWidget;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimationAsset>  FireAnimation;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AProjectile>  ProjectileClass;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AShellCasing>  CasingClass;
	
	UPROPERTY(ReplicatedUsing=OnRep_WeaponState, VisibleAnywhere, Category = "Weapon Properties")
	EWeaponState WeaponState;

	

};
