// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Widgets/CrosshairWidgetComponent.h"
#include "UI/TurretCrosshairWidget.h"

void UCrosshairWidgetComponent::InitiateReload(float ReloadTime)
{
	if (UTurretCrosshairWidget* CrosshairWidget = Cast<UTurretCrosshairWidget>(GetUserWidgetObject()))
	{
		CrosshairWidget->InitiateReload(ReloadTime);
	}
}
