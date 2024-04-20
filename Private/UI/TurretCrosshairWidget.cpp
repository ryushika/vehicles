// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TurretCrosshairWidget.h"
#include "UI/RoundProgressBar.h"
#include "Types.h"

void UTurretCrosshairWidget::InitiateReload(float ReloadTime)
{
	TickReloadTime = 0.f;
	bIsReloading = true;
	CurrentReloadTime = ReloadTime;
	StaticCircle->SetColorAndOpacity(FLinearColor::Green);
}

void UTurretCrosshairWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	if (bIsReloading)
	{
		TickReloadTime += InDeltaTime;
		Reloading(CurrentReloadTime);
	}
}

void UTurretCrosshairWidget::Reloading(float ReloadTime)
{
	if (ReloadCircle)
	{
		ReloadCircle->SetPercent(TickReloadTime / ReloadTime);
		if (TickReloadTime >= ReloadTime)
		{
			bIsReloading = false;
			TickReloadTime = 0.f;
			CurrentReloadTime = 0.f;
			ReloadCircle->SetPercent(0.f);
			StaticCircle->SetColorAndOpacity(FLinearColor::White);
		}
	}
}
