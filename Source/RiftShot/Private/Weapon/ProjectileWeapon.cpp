
#include "Weapon/ProjectileWeapon.h"

#include "Weapon/Projectile.h"


AProjectileWeapon::AProjectileWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AProjectileWeapon::Fire(FVector TargetLocation)
{
	
	APawn* InstigatorPawn = Cast<APawn>(GetOwner());
	
	if (HasAuthority())
	{
		FVector SpawnLocation = SkeletalMeshComponent->GetSocketLocation(FName("Muzzle"));

		FVector Direction = (TargetLocation - SpawnLocation).GetSafeNormal();
		FRotator LookAtRotation = Direction.Rotation();

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = GetOwner();
		SpawnParams.Instigator = InstigatorPawn;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		if (UWorld* World = GetWorld())
		{
			World->SpawnActor<AProjectile>(ProjectileClass, SpawnLocation, LookAtRotation, SpawnParams);
		}
	}
	
		Super::Fire(TargetLocation);
	
}

void AProjectileWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void AProjectileWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

