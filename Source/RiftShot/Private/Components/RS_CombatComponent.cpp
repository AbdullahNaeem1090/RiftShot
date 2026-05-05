
#include "Components/RS_CombatComponent.h"
#include "Characters/RS_Character.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Weapon//RS_BaseWeapon.h"


URS_CombatComponent::URS_CombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void URS_CombatComponent::BeginPlay()
{
	Super::BeginPlay();
}

void URS_CombatComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(URS_CombatComponent, EquippedWeapon);
	DOREPLIFETIME(URS_CombatComponent, bIsAiming);
}

void URS_CombatComponent::PerformLineTrace()
{
	FVector2D ViewportSize;
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}
	
	FVector2D CrosshairLocation(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);
	
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;
	
	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
		UGameplayStatics::GetPlayerController(this, 0),
		CrosshairLocation,
		CrosshairWorldPosition,
		CrosshairWorldDirection
	);
	
	if (bScreenToWorld)
	{
		FVector Start = CrosshairWorldPosition;

		FVector End = Start + CrosshairWorldDirection * 3000;

		GetWorld()->LineTraceSingleByChannel(
			TraceHitResult,
			Start,
			End,
			ECollisionChannel::ECC_Visibility
		);
		if (!TraceHitResult.bBlockingHit)
		{
			TraceHitResult.ImpactPoint = End;
		}
	}
}

void URS_CombatComponent::SetAiming(bool bValue)
{
	bIsAiming=bValue;
	ServerSetAiming(bValue);
}

void URS_CombatComponent::ServerSetAiming_Implementation(bool bValue)
{
	bIsAiming=bValue;
}

void URS_CombatComponent::OnRep_EquippedWeapon()
{
	if (Character && EquippedWeapon)
		Character->SetRotationMode(ERS_RotationMode::Strafe);
}

void URS_CombatComponent::Equip(ARS_BaseWeapon* InWeapon)
{
	if (!IsValid(Character)||!IsValid(InWeapon)) return;
	
	EquippedWeapon=InWeapon;
	
	const bool Attached = EquippedWeapon->AttachToComponent(
		Character->GetMesh(),
		FAttachmentTransformRules::SnapToTargetNotIncludingScale,
		FName(TEXT("WeaponSocketRight"))
		);
	
	if (!Attached) return;
	
	Character->SetNewAnimLayer();
	
	EquippedWeapon->SetWeaponState(EWeaponState::Ews_Equipped);
	EquippedWeapon->SetOwner(Character);
	Character->SetRotationMode(ERS_RotationMode::Strafe);
	
}

void URS_CombatComponent::FireButtonPressed(bool bPressed)
{
	bFireButtonPressed = bPressed;
	if (EquippedWeapon == nullptr) return;
	if (Character && bFireButtonPressed)
	{
		PerformLineTrace();
		
		ServerFire(TraceHitResult.ImpactPoint);
	}
}

void URS_CombatComponent::ServerFire_Implementation(const FVector_NetQuantize& TargetLocation)
{
	MulticastFire(TargetLocation);
}

void URS_CombatComponent::MulticastFire_Implementation(const FVector_NetQuantize& TargetLocation)
{
	Character->PlayFireMontage(bIsAiming);
	EquippedWeapon->Fire(TargetLocation);
}





