// Fill out your copyright notice in the Description page of Project Settings.


#include "PC_Base.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Blueprint/WidgetBlueprintLibrary.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Blueprint/WidgetLayoutLibrary.h>

void APC_Base::BeginPlay()
{
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(this);
}

void APC_Base::CleanUp_UI()
{
	UWidgetLayoutLibrary::RemoveAllWidgets(GetWorld());
}
