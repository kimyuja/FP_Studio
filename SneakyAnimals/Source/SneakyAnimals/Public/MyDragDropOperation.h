// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "MyDragDropOperation.generated.h"

/**
 * 
 */
UCLASS()
class SNEAKYANIMALS_API UMyDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UW_ItemImg* widgetReference;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D dragOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UItemObject* draggedItemObj;
	
};
