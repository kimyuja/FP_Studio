// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemComponent.h"
#include "MapCustomWidget.generated.h"

/**
 * 
 */
UCLASS()
class SNEAKYANIMALS_API UMapCustomWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;

protected:
    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "UI")
    class UW_CustomMap* CustomMapWidget;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "UI")
	float tileSize;
};
