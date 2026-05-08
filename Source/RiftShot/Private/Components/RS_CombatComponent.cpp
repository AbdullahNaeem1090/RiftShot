
#include "Components/RS_CombatComponent.h"

#include "Camera/CameraComponent.h"
#include "Characters/RS_Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interfaces/CrossHairInteract.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Player/RS_PlayerController.h"
#include "Weapon//RS_BaseWeapon.h"
#include "Widgets/RS_HUD.h"


URS_CombatComponent::URS_CombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void URS_CombatComponent::BeginPlay()
{
	Super::BeginPlay();
	if (Character)
	{
		DefaultFOV=Character->GetCameraComponent()->FieldOfView;
		CurrentFOV=DefaultFOV;
	}
}

void URS_CombatComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(URS_CombatComponent, EquippedWeapon);
	DOREPLIFETIME(URS_CombatComponent, bIsAiming);
}

void URS_CombatComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	PerformLineTrace();
	if (Character && Character->IsLocallyControlled())
	{
		SetHudCrossHairs(DeltaTime);
		UpdateFOV(DeltaTime);
	}
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
		const float DistanceToCharacter=(Character->GetActorLocation() - Start).Size();
		Start = Start + CrosshairWorldDirection * (DistanceToCharacter+100.f);

		const FVector End = Start + CrosshairWorldDirection * 3000;

		GetWorld()->LineTraceSingleByChannel(
			TraceHitResult,
			Start,
			End,
			ECollisionChannel::ECC_Visibility
		);
		if (!TraceHitResult.bBlockingHit)
		{
			HitTargetLocation=End;
			TraceHitResult.ImpactPoint=End;
		}
		else
		{
			HitTargetLocation=TraceHitResult.ImpactPoint;
		}
	}
}

void URS_CombatComponent::SetAiming(bool bValue)
{
	bIsAiming=bValue;
	ServerSetAiming(bValue);
}

void URS_CombatComponent::SetHudCrossHairs(float DeltaTime)
{
	if (!Character || Character->Controller == nullptr || !EquippedWeapon) return;

	if (!Controller)
		Controller = Cast<ARS_PlayerController>(Character->Controller);
	
	if (!HUD)
		HUD = Cast<ARS_HUD>(Controller->GetHUD());
	
	if (Controller && HUD)
	{
			FHUDPackage HUDPackage;
			if (EquippedWeapon)
			{
				HUDPackage.CrosshairsCenter = EquippedWeapon->CrosshairsCenter;
				HUDPackage.CrosshairsLeft = EquippedWeapon->CrosshairsLeft;
				HUDPackage.CrosshairsRight = EquippedWeapon->CrosshairsRight;
				HUDPackage.CrosshairsBottom = EquippedWeapon->CrosshairsBottom;
				HUDPackage.CrosshairsTop = EquippedWeapon->CrosshairsTop;
			}
			else
			{
				HUDPackage.CrosshairsCenter = nullptr;
				HUDPackage.CrosshairsLeft = nullptr;
				HUDPackage.CrosshairsRight = nullptr;
				HUDPackage.CrosshairsBottom = nullptr;
				HUDPackage.CrosshairsTop = nullptr;
			}
		
		
		float CurrentSpeed=Character->GetVelocity().Size2D();
		
		float SpreadMagnitude= FMath::GetMappedRangeValueClamped(
			FVector2D(0, Character->GetCharacterMovement()->MaxWalkSpeed),
			FVector2D(0, 1),
			CurrentSpeed
			);
		
	
		AActor* HitActor = TraceHitResult.GetActor();

		if (HitActor && HitActor->Implements<UCrossHairInteract>())
		{
			HUDPackage.CrosshairColor = FLinearColor::Red;
		}
		else
		{
			HUDPackage.CrosshairColor = FLinearColor::White;
		}
		
		HUDPackage.CrosshairSpread=SpreadMagnitude;
		
		HUD->SetHUDPackage(HUDPackage);
	}
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

void URS_CombatComponent::UpdateFOV(float DeltaTime)
{
	if (Character && EquippedWeapon)
	{
		if (bIsAiming)
		{
			CurrentFOV=FMath::FInterpTo(CurrentFOV,EquippedWeapon->ZoomFOV,DeltaTime,EquippedWeapon->ZoomInterpSpeed);
		}
		else
		{
			CurrentFOV=FMath::FInterpTo(CurrentFOV,DefaultFOV,DeltaTime,30.f);
		}
		Character->GetCameraComponent()->SetFieldOfView(CurrentFOV);
	}
}

void URS_CombatComponent::Fire()
{
	if (bCanFire)
	{
		bCanFire=false;
		
		ServerFire(TraceHitResult.ImpactPoint);
		
		StartFireTimer();
	}

}

void URS_CombatComponent::FireButtonPressed(bool bPressed)
{
	bFireButtonPressed = bPressed;
	if (EquippedWeapon == nullptr) return;
	if (Character && bFireButtonPressed)
	{
		Fire();
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




void URS_CombatComponent::StartFireTimer()
{
	if (Character && EquippedWeapon)
	{
		Character->GetWorldTimerManager().SetTimer(
			FireTimer,
			this,
			&URS_CombatComponent::FireTimerFinished,
			EquippedWeapon->FireDelay
		);
	}
}

void URS_CombatComponent::FireTimerFinished()
{
	if (EquippedWeapon == nullptr) return;
	bCanFire = true;
	if (bFireButtonPressed && EquippedWeapon->bAutomatic)
	{
		Fire();
	}
}

