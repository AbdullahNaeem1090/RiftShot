// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/OverheadWidget.h"
#include "Components/TextBlock.h"

void UOverheadWidget::SetDisplayText(const FString& Text)
{
	DisplayText->SetText(FText::FromString(Text));
}

void UOverheadWidget::ShowDisplayText(APawn* InPawn)
{
	if (InPawn == nullptr) return;
	
	const UEnum* Enum=StaticEnum<ENetRole>();
	
	const FString LRole=Enum->GetNameStringByValue((int64)InPawn->GetLocalRole());
	const FString RRole=Enum->GetNameStringByValue((int64)InPawn->GetRemoteRole());
	
	const FString Text = FString::Printf(TEXT("Local : %s - Remote : %s"), *LRole, *RRole);
	
	SetDisplayText(Text);

}
