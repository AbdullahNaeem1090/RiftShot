
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RS_Character.generated.h"

class UWidgetComponent;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class RIFTSHOT_API ARS_Character : public ACharacter
{
	GENERATED_BODY()

public:
	ARS_Character();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

public:	
	
protected:
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> SpringArmComponent;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> CameraComponent;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,meta=(AllowPrivateAccess=true))
	TObjectPtr<UWidgetComponent> OverHeadWidgetComponent;


};
