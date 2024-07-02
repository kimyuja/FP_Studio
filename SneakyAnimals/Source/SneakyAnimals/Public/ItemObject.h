// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ItemObject.generated.h"

/**
 * 
 */
UCLASS()
class SNEAKYANIMALS_API UItemObject : public UObject
{
	GENERATED_BODY()

public:
	// 개체의 크기를 나타내는 변수 -> 그리드 내부의 실제 크기 추적
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FIntPoint dimensions;

	UFUNCTION(BlueprintCallable)
	FIntPoint GetDimensions() const;
	
}; 
