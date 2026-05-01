
#include "Characters/RS_Character.h"
#include "Camera/CameraComponent.h"
#include "Components/RS_CombatComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Interfaces/RS_AnimInterface.h"
#include "Net/UnrealNetwork.h"
#include  "Components/CapsuleComponent.h"
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
	
	SetRotationMode(ERS_RotationMode::OrientToMovement);
	GetCharacterMovement()->NavAgentProps.bCanCrouch=true;
	
	PreviousGait=ERS_Gait::Jog;
	CurrentGait=ERS_Gait::Jog;
	
	SetNetUpdateFrequency(66.f);
	SetMinNetUpdateFrequency(33.f);

}

void ARS_Character::BeginPlay()
{
	Super::BeginPlay();
	
	SetNewAnimLayer();

}

void ARS_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	DistanceToGround=GetGroundDistance();
	
	if (IsLocallyControlled())
	{
		float Pitch = GetBaseAimRotation().Pitch;
		
		if (Pitch > 90.f)
		{
			Pitch -= 360.f;
		}

		Server_SetAimPitch(Pitch);
	} //Pitch
	
}

void ARS_Character::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(ARS_Character,OverlappingWeapon,COND_OwnerOnly);
	DOREPLIFETIME(ARS_Character,PreviousGait);
	DOREPLIFETIME(ARS_Character,CurrentGait);
	DOREPLIFETIME(ARS_Character, AimPitch);
	DOREPLIFETIME(ARS_Character, CurrentLayer);
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

void ARS_Character::SetRotationMode(ERS_RotationMode Mode)
{
	RotationMode=Mode;
	switch (RotationMode)
	{
	case ERS_RotationMode::OrientToMovement:
		RotationMode=Mode;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		bUseControllerRotationYaw = false;
		break;
	case ERS_RotationMode::Strafe:
		GetCharacterMovement()->bOrientRotationToMovement = false;
		bUseControllerRotationYaw = true;
		
	}
}

void ARS_Character::SetNewAnimLayer()
{
	
	if (GetMesh())
	{
		if (IsWeaponEquipped())
		{
			CurrentLayer=RifleLayer;
			GetMesh()->LinkAnimClassLayers(RifleLayer);
		}else
		{
			CurrentLayer=UnArmedLayer;
			GetMesh()->LinkAnimClassLayers(UnArmedLayer);
		}
	}
}

bool ARS_Character::IsWeaponEquipped() const
{
	return (CombatComponent && CombatComponent->EquippedWeapon);
}

void ARS_Character::EquipPressed()
{
	if (CombatComponent )
	{
		if (HasAuthority()) CombatComponent->Equip(OverlappingWeapon);
		else ServerEquipPressed();
	}
}

bool ARS_Character::IsAiming() const
{
	return (CombatComponent && CombatComponent->bIsAiming);
}

void ARS_Character::StartAiming()
{
	if (CombatComponent && IsWeaponEquipped()) CombatComponent->SetAiming(true);
}

void ARS_Character::StopAiming()
{
	if (CombatComponent) CombatComponent->SetAiming(false);
}

void ARS_Character::FireStart()
{
	if (CombatComponent && CombatComponent->EquippedWeapon)
		CombatComponent->FireButtonPressed(true);
}

void ARS_Character::FireStop()
{
	if (CombatComponent && CombatComponent->EquippedWeapon)
		CombatComponent->FireButtonPressed(false);
}

void ARS_Character::OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	Super::OnStartCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
	
	if (HasAuthority()) SetGait(ERS_Gait::Crouch);
}

void ARS_Character::OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	Super::OnEndCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
	
	if (HasAuthority()) SetGait(PreviousGait);
	
}

void ARS_Character::OnRep_OverlappingWeapon(ARS_BaseWeapon* LastWeapon)
{
	if (LastWeapon) LastWeapon->SetPickUpWidgetVisibility(false);
	if (OverlappingWeapon) OverlappingWeapon->SetPickUpWidgetVisibility(true);
}

void ARS_Character::OnRep_AnimLayerChanged()
{
	GetMesh()->LinkAnimClassLayers(CurrentLayer);
}


void ARS_Character::ServerEquipPressed_Implementation()
{
	if (CombatComponent) CombatComponent->Equip(OverlappingWeapon);
}

void ARS_Character::ToggleCrouch()
{
	bIsCrouched ? UnCrouch() : Crouch();
}

void ARS_Character::SetGait(ERS_Gait NewGait)
{
	PreviousGait=CurrentGait;
	CurrentGait=NewGait;
	ApplyGaitToAnimInstance();
}

void ARS_Character::Server_SetAimPitch_Implementation(float NewPitch)
{
	AimPitch = NewPitch;
}

void ARS_Character::OnRep_GaitChanged()
{
	ApplyGaitToAnimInstance();
}

void ARS_Character::ApplyGaitToAnimInstance()
{
	if (UAnimInstance* Anim = GetMesh() ? GetMesh()->GetAnimInstance() : nullptr)
	{
		if (Anim->Implements<URS_AnimInterface>())
		{
			IRS_AnimInterface::Execute_UpdateGait(Anim,CurrentGait);
		}
	}
}


float ARS_Character::GetGroundDistance() const
{
	if (!GetWorld() || !GetCapsuleComponent())
		return -1.f;

	FVector ActorLocation = GetActorLocation();

	FVector Start = ActorLocation + FVector(0.f, 0.f, GetCapsuleComponent()->GetScaledCapsuleHalfHeight());

	FVector End = ActorLocation + FVector(0.f, 0.f, 3000.f);

	FHitResult Hit;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	bool bHit = GetWorld()->SweepSingleByChannel(
		Hit,
		Start,
		End,
		FQuat::Identity,
		ECC_Visibility,
		FCollisionShape::MakeSphere(10.f), 
		Params
	);

	if (bHit)
	{
		return Hit.Distance;
	}

	return -1.f;
}

void ARS_Character::PlayFireMontage(bool bIsAiming)
{
	if (CombatComponent && IsWeaponEquipped())
	{
		if (UAnimInstance* AnimInstance=GetMesh()->GetAnimInstance())
		{
			AnimInstance->Montage_Play(FireMontage);
			
			static const FName FireADS(TEXT("Fire_ADS"));
			static const FName FireHip(TEXT("Fire_Hip"));
			const FName MontageSection = bIsAiming ? FireADS : FireHip;
			
			AnimInstance->Montage_JumpToSection(MontageSection);
		}
	}
}
