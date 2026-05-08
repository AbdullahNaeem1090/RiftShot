// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/RS_HUD.h"

void ARS_HUD::DrawHUD()
{
	Super::DrawHUD();
	
	if (GEngine)
	{
		FVector2D ViewportSize;
		
		GEngine->GameViewport->GetViewportSize(ViewportSize);
		const FVector2D ViewportCenter(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);

		float SpreadOffset = SpreadMagnitude * HUDPackage.CrosshairSpread;
		FLinearColor Color = HUDPackage.CrosshairColor;
		
		if (HUDPackage.CrosshairsCenter)
		{
			const FVector2D Spread(0.f,0.f);
			DrawCrosshair(HUDPackage.CrosshairsCenter,Spread, ViewportCenter,Color);
		}
		if (HUDPackage.CrosshairsLeft)
		{
			const FVector2D Spread(-SpreadOffset,0.f);
			DrawCrosshair(HUDPackage.CrosshairsLeft,Spread, ViewportCenter,Color);
		}
		if (HUDPackage.CrosshairsRight)
		{
			const FVector2D Spread(SpreadOffset,0.f);
			DrawCrosshair(HUDPackage.CrosshairsRight,Spread, ViewportCenter,Color);
		}
		if (HUDPackage.CrosshairsTop)
		{
			const FVector2D Spread(0.f,-SpreadOffset);
			DrawCrosshair(HUDPackage.CrosshairsTop,Spread ,ViewportCenter,Color);
		}
		if (HUDPackage.CrosshairsBottom)
		{
			const FVector2D Spread(0.f,SpreadOffset);
			DrawCrosshair(HUDPackage.CrosshairsBottom,Spread, ViewportCenter,Color);
		}
	}
}

void ARS_HUD::DrawCrosshair(UTexture2D* Texture, FVector2D Spread, FVector2D ViewportCenter,FLinearColor Color)
{
	const float TextureWidth = Texture->GetSizeX();
	const float TextureHeight = Texture->GetSizeY();
	const FVector2D TextureDrawPoint(
		ViewportCenter.X - (TextureWidth / 2.f),
		ViewportCenter.Y - (TextureHeight / 2.f)
	);

	DrawTexture(
		Texture,
		TextureDrawPoint.X + Spread.X,
		TextureDrawPoint.Y + Spread.Y,
		TextureWidth,
		TextureHeight,
		0.f,
		0.f,
		1.f,
		1.f,
		Color
	);
}
