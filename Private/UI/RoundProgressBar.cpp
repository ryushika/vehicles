// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RoundProgressBar.h"
#include "Components/Image.h"
#include "Kismet/KismetMaterialLibrary.h"

void URoundProgressBar::SetPercent(float Percent)
{
	if (ProgressBarImage && ProgressBarMaterial)
	{
		if (ProgressBarInstance)
		{
			ProgressBarInstance->SetScalarParameterValue("Percent", Percent);
		}
		else
		{
			ProgressBarInstance = UKismetMaterialLibrary::CreateDynamicMaterialInstance(this, ProgressBarMaterial);
			ProgressBarImage->SetBrushFromMaterial(ProgressBarInstance);
			SetPercent(Percent);
		}
	}
}
