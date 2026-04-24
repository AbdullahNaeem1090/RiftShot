
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RS_Character.generated.h"

class ARS_BaseWeapon;
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
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

public:	
	void SetOverlappingWeapon(ARS_BaseWeapon* InWeapon);
	
protected:
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> SpringArmComponent;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> CameraComponent;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,meta=(AllowPrivateAccess=true))
	TObjectPtr<UWidgetComponent> OverHeadWidgetComponent;
	
	UPROPERTY(ReplicatedUsing=OnRep_OverlappingWeapon)
	TObjectPtr<ARS_BaseWeapon> OverlappingWeapon;
	
private:
	UFUNCTION()
	void OnRep_OverlappingWeapon(ARS_BaseWeapon* LastWeapon);


};
