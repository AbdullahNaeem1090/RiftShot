
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShellCasing.generated.h"

UCLASS()
class RIFTSHOT_API AShellCasing : public AActor
{
	GENERATED_BODY()

public:
	AShellCasing();

protected:
	virtual void BeginPlay() override;
	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> CasingMesh;

	UPROPERTY(EditAnywhere)
	float ShellEjectionImpulse;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> ShellSound;
};
