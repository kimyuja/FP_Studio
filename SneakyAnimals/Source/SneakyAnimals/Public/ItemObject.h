// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ItemObject.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class SNEAKYANIMALS_API UItemObject : public UObject
{
	GENERATED_BODY()

public:
	// 개체의 크기를 나타내는 변수 -> 그리드 내부의 실제 크기 추적
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item")
	FIntPoint dimensions;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item")
	UMaterialInterface* icon;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item")
	UMaterialInterface* iconRotated90;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item")
	UMaterialInterface* iconRotated180;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item")
	UMaterialInterface* iconRotated270;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item")
	bool bRotated;

	// AGimmic 클래스를 class reference 타입으로 변수 생성
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item")
	TSubclassOf<class AGimmick> itemClass;

	UFUNCTION(BlueprintCallable)
	FIntPoint GetDimensions(UItemObject* _itemObj);
	
	UFUNCTION(BlueprintCallable)
	UMaterialInterface* GetIcon() const;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item")
	int32 itemActiveType;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item")
	int32 setWorldActorRot;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Item")
	FVector beginLoc;

	UPROPERTY(BlueprintReadWrite)
	float rotationAngle;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Item")
	bool bIsDraggable;

	UFUNCTION()
	void RotateGA(class UItemObject* _itemObj);

	UFUNCTION()
	float IsRotatedGA();

	UFUNCTION()
	void SetDimensions(const FIntPoint& NewDimensions);

	UPROPERTY(BlueprintReadWrite)
	FIntPoint currentDimensions;

	UPROPERTY(BlueprintReadWrite)
	int32 rotationImgCheck;


	UPROPERTY(BlueprintReadWrite)
	FRotator beginRot;

}; 
