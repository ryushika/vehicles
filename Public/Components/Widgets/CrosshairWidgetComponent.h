// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "CrosshairWidgetComponent.generated.h"


UCLASS()
class TANKS_API UCrosshairWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
//  Functions

public:
	void InitiateReload(float ReloadTime);

protected:

private:

//  Variables

public:

protected:

private:
};
