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

	/*tiles.Reset(0);

	tiles.Add(FTileStructureTemp(0, 0));

	for (int32 i = 1; i < arraySize; i++)
	{
		tiles.Add(FTileStructureTemp(i % columns, i / columns));
	}*/

	// items �迭 resize
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

// TryAddItem�� ��ư�� ������ �������� ��ư�� itemObject�� �޾� ȣ�������� ��
bool UItemComponent::TryAddItem(UItemObject* _ItemObject)
{
	// �׸��忡 �߰��Ϸ��� �������� �ް� �� �������� ��ȿ���� Ȯ��
	if (IsValid(_ItemObject))
	{
		// leftTop ���� ������ rightBottom ���� �׸����� �� Ÿ�Ͽ� ���� �ش� Ÿ�� �ֺ��� �߰��� �������� ��ġ�� �� �ִ� ��ŭ�� ������ �ִ��� Ȯ��  
		for (int32 idx = 0; idx < items.Num(); idx++)
		{
			bool bRoomAvailable = IsRoomAvailable(_ItemObject, idx);
			UE_LOG(LogTemp, Warning, TEXT("idx: %d"), idx);

			// ������ �ִٸ� �ε����� �׸� �߰��ϰ� true ��ȯ
			if (bRoomAvailable)
			{
				AddItemAt(_ItemObject, idx);
				return true;
			}
		}
		return false;
	}
	return false;
}

bool UItemComponent::IsRoomAvailable(UItemObject* __ItemObject, int32 __TopLeftIndex)
{
	// itemObject�� ���� ������ �����ϴ��� ���� Ȯ��
	FIntPoint itemDimension = __ItemObject->GetDimensions();
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


	// item�� leftTop�� �ε����� �߰��� ��� �����ϰ� �� �κ��丮 ������ Ÿ�� �ݺ� Ž��
	for (int32 i = topLeftTile.X; i < iLastIdx; i++)
	{
		for (int32 j = topLeftTile.Y; j < jLastIdx; j++)
		{

			FTileStructureTemp tile;
			tile.X = i;
			tile.Y = j;

			// Ÿ���� ��ȿ���� Ȯ��
			// return ���� false�� ��� : ��� ������ ���� ����
			// return ���� true�� ���(��ȿ�� ���) : ��� ������ ���� ���� => �ش� Ÿ�� ��ȯ
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
		//// GetItemAtIndex�� itemObject�� ������
		//// CheckIsValid() 
		//if (IsValid(GetItemAtIndex(TileToIndex(tile))))
		//{
		//	UE_LOG(LogTemp, Warning, TEXT("isvaild"));
		//	return false;
		//}// **********************
		//else {
		//	if (items.IsValidIndex(TileToIndex(tile)))
		//	{
		//		if (items[TileToIndex(tile)] == nullptr)
		//		{
		//			return true;
		//		}
		//		else
		//		{
		//			return false;
		//		}
		//	}
		//	else
		//	{
		//		return false;
		//	}
		//}

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
	// 0 ���� n���� x�� y ��ǥ�� ��ȯ
	return FTileStructureTemp(Index % columns, Index / columns);
}

bool UItemComponent::IsTileValid(FTileStructureTemp Tile) const
{
	// Ÿ���� ��ȿ���� ���θ� �˷��ִ� ��ũ��

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
	/*bool bValid = items.IsValidIndex(Index);

	UItemObject* ItemObjectTemp = items[Index];

	if (!bValid)
	{
		CheckIsValid(false);
		return nullptr;
	}
	else
	{
		CheckIsValid(true);
		return ItemObjectTemp;
	}*/

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
	FIntPoint itemDimension = _ItemObject_->GetDimensions();
	FTileStructureTemp topLeftTile = IndexToTile(_TopLeftIndex_);

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

			// setArrayElem���� Items�� targetArray�� �ް� itemIdx�� Index�� �ް� ItmeObject�� Item���� ����
			// items.Insert(_ItemObject_, itemIdx);
			items[itemIdx] = _ItemObject_;
		}
	}
	isDirty = true;
}

TMap<UItemObject*, FTileStructureTemp> UItemComponent::GetAllItems()
{
	TMap<UItemObject*, FTileStructureTemp> AllItems;

	// �迭�� ��� �׸� �Ǵ� ��� Ÿ���� �ݺ��ϰ� ������ �׸��� �ִ��� Ȯ��
	for (int32 idx = 0; idx < items.Num(); idx++)
	{
		UItemObject* currentItemObject = items[idx];

		if (IsValid(currentItemObject))
		{
			FTileStructureTemp currentTile = IndexToTile(idx);

			// �ʿ� �׸� �߰� ���� Ȯ��
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
