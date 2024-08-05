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

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* moniter1;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* moniter2;

	UPROPERTY(EditAnywhere)
	class UTexture2D* warningScreen;

	UPROPERTY(EditAnywhere)
	class UTexture2D* sosScreen;

	UFUNCTION()
	void SetWarning();

	UFUNCTION()
	void SetHelp();


};
