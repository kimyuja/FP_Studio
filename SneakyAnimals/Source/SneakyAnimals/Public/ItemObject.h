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
	// ��ü�� ũ�⸦ ��Ÿ���� ���� -> �׸��� ������ ���� ũ�� ����
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FIntPoint dimensions;

	UFUNCTION(BlueprintCallable)
	FIntPoint GetDimensions() const;
	
}; 
