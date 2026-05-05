
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class UProjectileMovementComponent;
class UBoxComponent;
class UParticleSystemComponent;
class UParticleSystem;

UCLASS()
class RIFTSHOT_API AProjectile : public AActor
{
	GENERATED_BODY()

public:
	AProjectile();

protected:
	virtual void BeginPlay() override;
	virtual void Destroyed() override;
	
	UFUNCTION()
	virtual void OnProjectileHit(
		UPrimitiveComponent* HitComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit
	);
	
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> CollisionBox;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UParticleSystemComponent> TracerParticleSystemComponent ;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UParticleSystem> TracerParticleSystem ;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UParticleSystem> ImpactParticleSystem ;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> ImpactSound  ;
	
};
