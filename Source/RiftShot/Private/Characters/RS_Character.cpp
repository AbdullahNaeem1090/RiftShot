
#include "Characters/RS_Character.h"
#include "Camera/CameraComponent.h"
#include "Components/RS_CombatComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"
#include "Weapon/RS_BaseWeapon.h"

ARS_Character::ARS_Character()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SpringArmComponent=CreateDefaultSubobject<USpringArmComponent>(FName("SpringArmComp"));
	SpringArmComponent->SetupAttachment(GetMesh());
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->TargetArmLength = 500.0f;
	
	CameraComponent=CreateDefaultSubobject<UCameraComponent>(FName("CameraComp"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->bUsePawnControlRotation=false;
	
	OverHeadWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
	OverHeadWidgetComponent->SetupAttachment(RootComponent);
	
	CombatComponent=CreateDefaultSubobject<URS_CombatComponent>("CombatComp");
	CombatComponent->SetIsReplicated(true);
	
	bUseControllerRotationYaw=false;
	GetCharacterMovement()->bOrientRotationToMovement=true;

}

void ARS_Character::BeginPlay()
{
	Super::BeginPlay();
}

void ARS_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARS_Character::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(ARS_Character,OverlappingWeapon,COND_OwnerOnly);
}

void ARS_Character::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	CombatComponent->Character=this;
}

void ARS_Character::SetOverlappingWeapon(ARS_BaseWeapon* InWeapon)
{
	if (IsLocallyControlled())
	{
		if (InWeapon) InWeapon->SetPickUpWidgetVisibility(true);	
		else if (OverlappingWeapon) OverlappingWeapon->SetPickUpWidgetVisibility(false);
	}
	OverlappingWeapon=InWeapon;
}

void ARS_Character::EquipPressed()
{
	if (CombatComponent && HasAuthority()) CombatComponent->Equip(OverlappingWeapon);
}

void ARS_Character::OnRep_OverlappingWeapon(ARS_BaseWeapon* LastWeapon)
{
	if (LastWeapon) LastWeapon->SetPickUpWidgetVisibility(false);
	if (OverlappingWeapon) OverlappingWeapon->SetPickUpWidgetVisibility(true);
}


