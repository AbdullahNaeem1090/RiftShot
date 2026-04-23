
#include "Characters/RS_Character.h"
#include "Camera/CameraComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

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


