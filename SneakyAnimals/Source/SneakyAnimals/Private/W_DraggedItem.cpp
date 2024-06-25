// Fill out your copyright notice in the Description page of Project Settings.


#include "W_DraggedItem.h"

void UW_DraggedItem::NativeConstruct()
{
	Super::NativeConstruct();
}

bool UW_DraggedItem::Initialize()
{
	bool Success = Super::Initialize();
	if(!Success) return false;

	// TODO: setup

	// itemImg가 널포인터면 초기화 실패 false return
	//if(!ensure(itemImg != nullptr)) return false;
	// 사실 이것도 안해도 될 것 같아
	// 위젯 자체가 안불려오는 경우를 생각해야 할 듯

	return true;
}