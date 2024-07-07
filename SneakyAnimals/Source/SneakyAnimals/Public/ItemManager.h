// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// UItemManager �̱��� ��ü ����� itemObject ���� ������ ����� ���

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ItemObject.h"
#include "ItemManager.generated.h"

/**
 * 
 */
UCLASS()
class SNEAKYANIMALS_API UItemManager : public UObject
{
	GENERATED_BODY()

public:
	static UItemManager* Get();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items")
	UItemObject* itemObject;

private:
	static UItemManager* Instance;
	
};
