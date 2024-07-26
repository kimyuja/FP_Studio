// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemComponent.h"
#include "ItemObject.h"
#include "TileStructure.h"
// #include <../../../../../../../Source/Runtime/CoreUObject/Public/UObject/NoExportTypes.h>

// Sets default values for this component's properties
UItemComponent::UItemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	columns = 10;
	rows = 10;
}


// Called when the game starts
void UItemComponent::BeginPlay()
{
	Super::BeginPlay();

	int32 arraySize = columns * rows;

	// items 배열 resize
	items.SetNum(arraySize);

}


// Called every frame
void UItemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	if (isDirty)
	{
		isDirty = false;

		OnInventoryChanged.Broadcast();

	}
}

// TryAddItem은 버튼을 누르는 시점에서 버튼의 itemObject를 받아 호출시켜줘야 해
bool UItemComponent::TryAddItem(UItemObject* _ItemObject)
{
	UE_LOG(LogTemp, Warning, TEXT("Start TryAddItem Func"));

	// 그리드에 추가하려는 아이템을 받고 이 아이템이 유효한지 확인
	if (IsValid(_ItemObject))
	{
		// leftTop 부터 시작해 rightBottom 까지 그리드의 각 타일에 대해 해당 타일 주변에 추가할 아이템을 배치할 수 있는 만큼의 공간이 있는지 확인  
		for (int32 idx = 0; idx < items.Num(); idx++)
		{
			bool bRoomAvailable = IsRoomAvailable(_ItemObject, idx);
			UE_LOG(LogTemp, Warning, TEXT("idx: %d"), idx);

			// 공간이 있다면 인덱스에 항목 추가하고 true 반환
			if (bRoomAvailable)
			{
				AddItemAt(_ItemObject, idx);
				return true;
			}
		}
		return false;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Tryed Item is not valid!"));
	}
	return false;
}

bool UItemComponent::IsRoomAvailable(UItemObject* __ItemObject, int32 __TopLeftIndex)
{
	// itemObject를 위한 공간이 존재하는지 여부 확인
	FIntPoint itemDimension = __ItemObject->GetDimensions(__ItemObject);
	FTileStructureTemp topLeftTile = IndexToTile(__TopLeftIndex);

	int32 iLastIdx = topLeftTile.X + itemDimension.X;
	int32 jLastIdx = topLeftTile.Y + itemDimension.Y;

	// bool bFind = false;
	UE_LOG(LogTemp, Warning, TEXT("XXYY : %d, %d, %d, %d"), topLeftTile.X, itemDimension.X, topLeftTile.Y, itemDimension.Y);

	if (iLastIdx > columns || jLastIdx > rows)
	{
		return false;
	}

	/*UE_LOG(LogTemp, Warning, TEXT("topLeftTile: %d"), topLeftTile.X);
	UE_LOG(LogTemp, Warning, TEXT("iLastIdx: %d"), iLastIdx);*/


	// item이 leftTop의 인덱스에 추가될 경우 차지하게 될 인벤토리 내부의 타일 반복 탐색
	for (int32 i = topLeftTile.X; i < iLastIdx; i++)
	{
		for (int32 j = topLeftTile.Y; j < jLastIdx; j++)
		{

			FTileStructureTemp tile;
			tile.X = i;
			tile.Y = j;

			// 타일이 유효한지 확인
			// return 값이 false인 경우 : 사용 가능한 공간 없음
			// return 값이 true인 경우(유효한 경우) : 사용 가능한 공간 존재 => 해당 타일 반환
			if (!CheckEmptySlot(tile))
			{
				return false;
			}

		}
	}

	return true;
}

bool UItemComponent::CheckEmptySlot(FTileStructureTemp tile)
{
	bool bCheck = IsTileValid(tile);
	if (bCheck == false)
	{
		return false;
	}
	else
	{

		if (GetItemAtIndex(TileToIndex(tile)))
		{
			if (IsValid(outItemTemp))
			{
				return false;
			}
			else
			{
				return true;
			}
		}
		else
		{
			return false;
		}
	}
}

FTileStructureTemp UItemComponent::IndexToTile(int32 Index) const
{
	// 0 에서 n까지 x와 y 좌표로 변환
	return FTileStructureTemp(Index % columns, Index / columns);
}

bool UItemComponent::IsTileValid(FTileStructureTemp Tile) const
{
	// 타일이 유효한지 여부를 알려주는 매크로

	bool condition1 = (Tile.X >= 0);
	bool condition2 = (Tile.Y >= 0);
	bool condition3 = (Tile.X < columns);
	bool condition4 = (Tile.Y < rows);

	bool allCondition = condition1 && condition2 && condition3 && condition4;

	return allCondition;
}

int32 UItemComponent::TileToIndex(FTileStructureTemp Tile) const
{
	return Tile.X + (Tile.Y * columns);
}

bool UItemComponent::GetItemAtIndex(int32 Index)
{
	bool bValid = items.IsValidIndex(Index);

	if (bValid)
	{
		outItemTemp = items[Index];
		return true;
	}
	else
	{
		outItemTemp = nullptr;
		return false;
	}


}

void UItemComponent::AddItemAt(UItemObject* _ItemObject_, int32 _TopLeftIndex_)
{
	FIntPoint itemDimension = _ItemObject_->GetDimensions(_ItemObject_);
	UE_LOG(LogTemp, Warning, TEXT("!!! ITEMCOMPONENT Dimension XY is %d %d"), itemDimension.X, itemDimension.Y)
	FTileStructureTemp topLeftTile = IndexToTile(_TopLeftIndex_);
	UE_LOG(LogTemp, Warning, TEXT("!!! topLeft tile X is %d"), topLeftTile.X);

	int32 iLastIdx = topLeftTile.X + itemDimension.X;
	int32 jLastIdx = topLeftTile.Y + itemDimension.Y;
	UE_LOG(LogTemp, Warning, TEXT("!!! iLastIdx is %d / jLastIdx is %d"), iLastIdx, jLastIdx);


	for (int32 i = topLeftTile.X; i < iLastIdx; i++)
	{
		for (int32 j = topLeftTile.Y; j < jLastIdx; j++)
		{
			FTileStructureTemp tile;
			tile.X = i;
			tile.Y = j;

			int32 itemIdx = TileToIndex(tile);

			// setArrayElem으로 Items를 targetArray로 받고 itemIdx를 Index로 받고 ItmeObject를 Item으로 받음
			items[itemIdx] = _ItemObject_;
		}
	}
	isDirty = true;
}

TMap<UItemObject*, FTileStructureTemp> UItemComponent::GetAllItems()
{
	TMap<UItemObject*, FTileStructureTemp> AllItems;

	// 배열의 모든 항목 또는 모든 타일을 반복하고 실제로 항목이 있는지 확인
	for (int32 idx = 0; idx < items.Num(); idx++)
	{
		UItemObject* currentItemObject = items[idx];

		if (IsValid(currentItemObject))
		{
			FTileStructureTemp currentTile = IndexToTile(idx);

			// 맵에 항목 추가 여부 확인
			if (!AllItems.Contains(currentItemObject))
			{
				AllItems.Add(currentItemObject, currentTile);
			}
		}
	}

	return AllItems;
}

void UItemComponent::RemoveItem(UItemObject* _ItemObject)
{
	if(IsValid(_ItemObject))
	{
		// items의 item와 드롭할 item이 일치하는지 확인하고 일치한다면
		// 기본적으로 아무것도 설정하지 않거나 무효화하기를 원한다
		for (int32 i = 0; i < items.Num(); i++)
		{
			if (items[i] == _ItemObject)
			{
				items[i] = nullptr;
			}
		}

		isDirty = true;
	}
}

void UItemComponent::ChangeInventory()
{
	isDirty = true;
}

bool UItemComponent::CheckIsValid(bool b)
{
	if (b == true) return true;
	else return false;
}
