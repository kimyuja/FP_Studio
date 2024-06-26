// Fill out your copyright notice in the Description page of Project Settings.


#include "W_ItemSlot.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Image.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Button.h>
#include "Slate/SlateBrushAsset.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "UITestCharacter.h"

void UW_ItemSlot::NativeConstruct()
{
	Super::NativeConstruct();

	if (itemImg) 
	{
		FSlateBrush brush;
		brush.SetResourceObject(itemImg);
		
		if (img)
		{
			img->SetBrush(brush);
		}
	}

	ACharacter* playerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	
	AUITestCharacter* uiTestPlayer = Cast<AUITestCharacter>(playerCharacter);

}

bool UW_ItemSlot::Initialize()
{

	bool Success = Super::Initialize();
	if (!Success) return false;

	if (!btn) return false;

	// 마우스가 클릭한 정확한 위치값 받아오기
	btn->SetClickMethod(EButtonClickMethod::PreciseClick);
	btn->SetTouchMethod(EButtonTouchMethod::PreciseTap);
	btn->IsFocusable = false;


	return true;
}
