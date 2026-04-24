
#include "Components/RS_CombatComponent.h"
#include "Characters/RS_Character.h"
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

void URS_CombatComponent::Equip(ARS_BaseWeapon* InWeapon)
{
	if (!IsValid(Character)||!IsValid(InWeapon)) return;
	
	EquippedWeapon=InWeapon;
	
	const bool Attached = EquippedWeapon->AttachToComponent(
		Character->GetMesh(),
		FAttachmentTransformRules::SnapToTargetNotIncludingScale,
		FName(TEXT("RightHandSocket"))
		);
	
	if (!Attached) return;
	
	EquippedWeapon->SetWeaponState(EWeaponState::Ews_Equipped);
	EquippedWeapon->SetOwner(Character);
	
}

