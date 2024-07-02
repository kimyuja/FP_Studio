// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SM_ComputerMoniter.generated.h"

/**
 * 
 */
UCLASS()
class SNEAKYANIMALS_API USM_ComputerMoniter : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct()override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime)override;
};
