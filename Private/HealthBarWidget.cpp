// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthBarWidget.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UHealthBarWidget::UpdateHP(FText HealthText, float NewPercentage)
{
	ProgressBar->SetPercent(NewPercentage);
	TextBlock->SetText(HealthText);
}
