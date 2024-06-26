// Fill out your copyright notice in the Description page of Project Settings.


#include "W_CustomMap.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Border.h>

void UW_CustomMap::NativeConstruct()
{
	Super::NativeConstruct();


}

bool UW_CustomMap::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (!gridBorder) return false;

	gridBorder->SetPadding(0.0f);
	return true;
}
