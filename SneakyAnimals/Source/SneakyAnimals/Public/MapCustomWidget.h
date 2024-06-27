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
	UFUNCTION(BlueprintCallable, Category = "UI")
	void InitializeCustomMap(UClass* customMapClass, UItemComponent* itemComponent, float tileSize);

protected:
    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "UI")
    class UW_CustomMap* CustomMapWidget;


};
