
#include "Weapon/RS_BaseWeapon.h"
#include "Characters/RS_Character.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"

ARS_BaseWeapon::ARS_BaseWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	
	SkeletalMeshComponent=CreateDefaultSubobject<USkeletalMeshComponent>(FName("WeaponMesh"));
	SetRootComponent(SkeletalMeshComponent);
	
	SkeletalMeshComponent->SetCollisionResponseToAllChannels(ECR_Block);
	SkeletalMeshComponent->SetCollisionResponseToChannel(ECC_Pawn,ECR_Ignore);
	SkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	SphereComponent=CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComponent->SetupAttachment(SkeletalMeshComponent);
	
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	PickUpWidget=CreateDefaultSubobject<UWidgetComponent>("PickUpWidget");
	PickUpWidget->SetupAttachment(SkeletalMeshComponent);
	
}

void ARS_BaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority())
	{
		SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		SphereComponent->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);
		SphereComponent->OnComponentBeginOverlap.AddDynamic(this,&ARS_BaseWeapon::OnSphereOverlap);
		SphereComponent->OnComponentEndOverlap.AddDynamic(this,&ThisClass::OnSphereEndOverlap);
	}
	SetPickUpWidgetVisibility(false);
	
}

void ARS_BaseWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	if (ARS_Character* Character=Cast<ARS_Character>(OtherActor))
	{
		Character->SetOverlappingWeapon(this);
	}
		
}

void ARS_BaseWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (ARS_Character* Character=Cast<ARS_Character>(OtherActor))
	{
		Character->SetOverlappingWeapon(nullptr);
	}
}

void ARS_BaseWeapon::SetPickUpWidgetVisibility(bool Visible)
{
	if (PickUpWidget) PickUpWidget->SetVisibility(Visible);
}

