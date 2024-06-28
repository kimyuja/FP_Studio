// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_Clock.generated.h"

/**
 * 
 */
UCLASS()
class SNEAKYANIMALS_API UW_Clock : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

protected:

	UPROPERTY(EditAnywhere, Category = "MySettings", meta = (BindWidget))
	class UTextBlock* Time_Text;

	UFUNCTION(BlueprintCallable, Category = "MySettings")
	FText Get_Time();
};
