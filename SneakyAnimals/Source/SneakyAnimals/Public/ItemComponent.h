// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryChangedDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SNEAKYANIMALS_API UItemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UItemComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Constants")
	int32 columns;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Constants")
	int32 rows;

	UFUNCTION(BlueprintCallable)
	bool TryAddItem(UItemObject* ItemObject);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Constants")
	TArray<class UItemObject*> items;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Constants")
	TArray<FTileStructureTemp> tiles;

	// 변경되어 상태가 바뀌었음을 의미
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Constants")
	bool isDirty = false;

	// 그리드 내 배치 가능한 공간
	UFUNCTION(BlueprintCallable)
	bool IsRoomAvailable(UItemObject* ItemObject, int32 TopLeftIndex);

	UFUNCTION(BlueprintCallable)
	bool CheckEmptySlot(FTileStructureTemp tile);

	// macro
	UFUNCTION(BlueprintCallable)
	bool ForEachIndex(UItemObject* ItemObject, int32 TopLeftIndex);

	//UFUNCTION(BlueprintCallable)  
	//class UTileStructure* IndexToTile(int32 Index) const;

	//// macro -> 타일이 유효한지 여부 확인
	//UFUNCTION(BlueprintCallable)
	//bool IsTileValid(UTileStructure* Tile);
	//
	//UFUNCTION(BlueprintCallable)
	//int TileToIndex(UTileStructure* Tile);

	UFUNCTION(BlueprintCallable)  
	FTileStructureTemp IndexToTile(int32 Index) const;

	// macro -> 타일이 유효한지 여부 확인
	UFUNCTION(BlueprintCallable)
	bool IsTileValid(FTileStructureTemp Tile) const;
	
	UFUNCTION(BlueprintCallable)
	int32 TileToIndex(FTileStructureTemp Tile) const; 

	UFUNCTION(BlueprintCallable)
	UItemObject* GetItemAtIndex(int32 Index);

	UFUNCTION(BlueprintCallable)
	//UItemObject* AddItemAt(UItemObject* ItemObject, int32 TopLeftIndex);
	void AddItemAt(UItemObject* ItemObject, int32 TopLeftIndex);

	UFUNCTION(BlueprintCallable, Category="Custom")
	TMap<UItemObject*, FTileStructureTemp> GetAllItems() const;

	UFUNCTION(BlueprintCallable)
	void RemoveItem(class UItemObject* _ItemObject);

	// 델리게이트
	FOnInventoryChangedDelegate OnInventoryChanged;

	UFUNCTION()
	void ChangeInventory();


	UFUNCTION()
	bool CheckIsValid(bool b);

	UPROPERTY(EditAnywhere)
	bool checkcheckbool;


};
