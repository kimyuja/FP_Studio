// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemComponent.h"
#include "ItemObject.h"
#include <TileStructure.h>
#include <../../../../../../../Source/Runtime/CoreUObject/Public/UObject/NoExportTypes.h>

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
	columns = 5;
	rows = 5;

	int32 arraySize = columns * rows;

	tiles.Reset(0);

	tiles.Add(FTileStructureTemp(0, 0));

	for (int32 i = 1; i < arraySize; i++)
	{
		tiles.Add(FTileStructureTemp(i % columns, i / columns));
	}
}


// Called every frame
void UItemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UItemComponent::TryAddItem(UItemObject* ItemObject)
{
	if (ItemObject)
	{
		for (UItemObject* item : items)
		{

		}
		return true;
	}
	return false;
}

bool UItemComponent::IsRoomAvailable(UItemObject* ItemObject, int32 TopLeftIndex)
{
	FIntPoint itemDimension = ItemObject->GetDimensions();
	FTileStructureTemp topLeftTile = IndexToTile(TopLeftIndex);

	int32 iLastIdx = topLeftTile.X + itemDimension.X;
	int32 jLastIdx = topLeftTile.Y + itemDimension.Y;

	bool bFind = false;

	for (int32 i = topLeftTile.X; i < iLastIdx; i++)
	{
		for (int32 j = topLeftTile.Y; j < jLastIdx; j++)
		{
			
			FTileStructureTemp tile;
			tile.X = i;
			tile.Y = j;

			bFind = CheckEmptySlot(tile);

			/*if(bFind == false)
				return false;*/

			if(bFind)
			{
				if (GetItemAtIndex(TileToIndex(tile)))
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
	bool condition3 = (Tile.X < 0);
	bool condition4 = (Tile.Y < 0);
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

	if (bValid)
	{
		return items[Index];
	}
	else
	{
		return nullptr;
	}

}

