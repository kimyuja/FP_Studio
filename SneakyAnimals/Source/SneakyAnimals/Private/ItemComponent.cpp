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
	columns = 5;
	rows = 5;
}


// Called when the game starts
void UItemComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	int32 arraySize = columns * rows;

	tiles.Reset(0);

	tiles.Add(FTileStructureTemp(0, 0));

	for (int32 i = 1; i < arraySize; i++)
	{
		tiles.Add(FTileStructureTemp(i % columns, i / columns));
	}

	items.SetNum(arraySize);
}


// Called every frame
void UItemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UItemComponent::TryAddItem(UItemObject* ItemObject)
{
	// 그리드에 추가하려는 아이템을 받고 이 아이템이 유효한지 확인
	if (ItemObject)
	{
		// leftTop 부터 시작해 rightBottom 까지 그리드의 각 타일에 대해 해당 타일 주변에 추가할 아이템을 배치할 수 있는 만큼의 공간이 있는지 확인  
		for (int32 idx = 0; idx < items.Num(); idx++)
		{
			bool bRoomAvailable = IsRoomAvailable(ItemObject, idx);

			// 공간이 있다면 인덱스에 항목 추가하고 true 반환
			if (bRoomAvailable)
			{
				AddItemAt(ItemObject, idx);

				return true;
			}
		}
		return false;
	}
	return false;
}

bool UItemComponent::IsRoomAvailable(UItemObject* ItemObject, int32 TopLeftIndex)
{
	// itemObject를 위한 공간이 존재하는지 여부 확인

	FIntPoint itemDimension = ItemObject->GetDimensions();
	FTileStructureTemp topLeftTile = IndexToTile(TopLeftIndex);

	int32 iLastIdx = topLeftTile.X + itemDimension.X;
	int32 jLastIdx = topLeftTile.Y + itemDimension.Y;

	bool bFind = false;

	// 항목이 leftTop의 인덱스에 추가될 경우 차지하게 될 인벤토리 내부의 타일 반복 탐색
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
			bFind = CheckEmptySlot(tile);

			/*if(bFind == false)
				return false;*/

			if (bFind)
			{
				// 유효한 타일의 인덱스를 확인하여
				// 인덱스 위치의 인벤토리가 완전히 비어있으면 true 반환 (인덱스 유효)
				// 
				if (GetItemAtIndex(TileToIndex(tile)) != nullptr)
				{
					return false; // 애매하다 이게 맞나

				}
			}
			else
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
		if (items.IsValidIndex(TileToIndex(tile)))
		{
			if (items[TileToIndex(tile)] == nullptr)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
}

bool UItemComponent::ForEachIndex(UItemObject* ItemObject, int32 TopLeftIndex)
{
	//FIntPoint itemDimension = ItemObject->GetDimensions();
	//FTileStructureTemp topLeftTile = IndexToTile(TopLeftIndex);

	//int32 iLastIdx = topLeftTile.X + itemDimension.X;
	//int32 jLastIdx = topLeftTile.Y + itemDimension.Y;

	//for (int32 i = topLeftTile.X; i < iLastIdx; i++)
	//{
	//	for (int32 j = topLeftTile.Y; j < jLastIdx; j++)
	//	{
	//		//
	//		FTileStructureTemp tile;
	//		tile.X = i;
	//		tile.Y = j;

	//		bool bCheck = IsTileValid(tile);
	//		if (bCheck == false)
	//		{
	//			return false;
	//		}
	//		else
	//		{
	//			if (items.IsValidIndex(TileToIndex(tile)))
	//			{
	//				if (items[TileToIndex(tile)] == nullptr)
	//				{
	//					return true;
	//				}
	//				else
	//				{
	//					return false;
	//				}
	//			}
	//			else
	//			{
	//				return false;
	//			}
	//		}
	//	}
	//}

	return true;
}

//UTileStructure* UItemComponent::IndexToTile(int32 Index) const
//{
//	// 0 에서 n까지 x와 y 좌표로 변환
//	UTileStructure* tile = NewObject<UTileStructure>();
//
//	tile->X = Index % columns;
//	tile->Y = Index / columns;
//
//	return tile;
//}
//
//bool UItemComponent::IsTileValid(UTileStructure* Tile)
//{
//	bool condition1 = (Tile->X >= 0);
//	bool condition2 = (Tile->Y >= 0);
//	bool condition3 = (Tile->X < 0);
//	bool condition4 = (Tile->Y < 0);
//	/*
//	// 이게 맞는거 같은데 영상에선 위에 코드를 쓴다..
//	bool condition3 = (Tile.X < columns);
//	bool condition4 = (Tile.Y < rows);*/
//
//	bool allCondition = condition1 && condition2 && condition3 && condition4;
//
//	return allCondition;
//}
//
//int UItemComponent::TileToIndex(UTileStructure* Tile)
//{
//	return Tile->X + (Tile->Y * columns);
//}

FTileStructureTemp UItemComponent::IndexToTile(int32 Index) const
{
	// 0 에서 n까지 x와 y 좌표로 변환
	return FTileStructureTemp(Index % columns, Index / columns);
}

bool UItemComponent::IsTileValid(FTileStructureTemp Tile)
{
	// 타일이 유효한지 여부를 알려주는 매크로

	bool condition1 = (Tile.X >= 0);
	bool condition2 = (Tile.Y >= 0);
	bool condition3 = (Tile.X < columns);
	bool condition4 = (Tile.Y < rows);
	/*
	// 이게 맞는거 같은데 영상에선 위에 코드를 쓴다..
	bool condition3 = (Tile.X < columns);
	bool condition4 = (Tile.Y < rows);*/

	bool allCondition = condition1 && condition2 && condition3 && condition4;

	return allCondition;
}

int32 UItemComponent::TileToIndex(FTileStructureTemp Tile)
{
	return Tile.X + (Tile.Y * columns);
}

UItemObject* UItemComponent::GetItemAtIndex(int32 Index)
{
	bool bValid = items.IsValidIndex(Index);

	UItemObject* ItemObject = items[Index];

	if (bValid)
	{
		return ItemObject;
	}
	else
	{
		return nullptr;
	}


	/*bValid = false;

	if (!items.IsValidIndex(Index))
	{
		return nullptr;
	}

	UItemObject* ItemObject = items[Index];

	if (IsValid(ItemObject))
	{
		bValid = true;
		return ItemObject;
	}

	return nullptr;*/

}

UItemObject* UItemComponent::AddItemAt(UItemObject* ItemObject, int32 TopLeftIndex)
{
	FIntPoint itemDimension = ItemObject->GetDimensions();
	FTileStructureTemp topLeftTile = IndexToTile(TopLeftIndex);

	int32 iLastIdx = topLeftTile.X + itemDimension.X;
	int32 jLastIdx = topLeftTile.Y + itemDimension.Y;

	for (int32 i = topLeftTile.X; i < iLastIdx; i++)
	{
		for (int32 j = topLeftTile.Y; j < jLastIdx; j++)
		{
			FTileStructureTemp tile;
			tile.X = i;
			tile.Y = j;

			int32 itemIdx = TileToIndex(tile);

			// setArrayElem으로 Items를 targetArray로 받고 itemIdx를 Index로 받고 ItmeObject를 Item으로 받음
			items.Insert(ItemObject, itemIdx);
		}
	}
	isDirty = true;

	return nullptr;
}

TMap<FTileStructureTemp, UItemObject*> UItemComponent::GetAllItems() const
{
	TMap<FTileStructureTemp, UItemObject*> AllItems;

	// 배열의 모든 항목 또는 모든 타일을 반복하고 실제로 항목이 있는지 확인
	for (int32 idx = 0; idx < items.Num(); idx++)
	{
		UItemObject* currentItem = items[idx];

		if(IsValid(currentItem))
		{
			FTileStructureTemp currentTile = IndexToTile(idx);

			// 맵에 항목 추가 여부 확인
			if(!AllItems.Contains(currentTile))
			{
				AllItems.Add(currentTile, currentItem);
			}
		}

	}

	return AllItems; 
}
