// Fill out your copyright notice in the Description page of Project Settings.


#include "W_Character_Customization.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Button.h>
#include "W_Base_Button.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/WidgetSwitcher.h>
#include <../../../../../../../Source/Runtime/SlateCore/Public/Styling/SlateTypes.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/WrapBox.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/DataTableFunctionLibrary.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Blueprint/WidgetBlueprintLibrary.h>
#include "W_Character_Customization_Item.h"
#include "PS_Lobby.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Image.h>

UW_Character_Customization::UW_Character_Customization(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// DT_Available_Characters 를 로드한다.
	static ConstructorHelpers::FObjectFinder<UDataTable> InGameAvailableCharactersTable(TEXT("/Game/KYJ/Data/DT_Available_Characters"));
	if (InGameAvailableCharactersTable.Succeeded())
	{
		DT_Available_Characters = InGameAvailableCharactersTable.Object;
	}

	// WB_Character_Customization_Item 위젯 블루프린트 클래스를 로드한다.
	static ConstructorHelpers::FClassFinder<UW_Character_Customization_Item> WidgetBPClass(TEXT("/Game/KYJ/Widgets/Lobby/WB_Character_Customization_Item.WB_Character_Customization_Item_C"));
	if (WidgetBPClass.Succeeded())
	{
		Character_Customization_Item_bp = WidgetBPClass.Class;
	}
}

void UW_Character_Customization::Switch_ActiveTab(int32 ActiveIndex)
{
	WidgetSwitcher->SetActiveWidgetIndex(ActiveIndex);
	switch (ActiveIndex)
	{
	case 0:
		Set_ActiveTab(Characters_Btn->Button);
		break;
	case 1:
		Set_ActiveTab(Accessoires_Btn->Button);
		break;
	default:
		break;
	}
}

void UW_Character_Customization::Set_ActiveTab(UButton* Button_Target)
{
	// Resets buttons to be unselected
	FButtonStyle btn_Style;
	FButtonStyle Characters_Btn_Style = Characters_Btn->Button->GetStyle();

	FSlateBrush EmptyBrush = CreateSlateBrushFromTexture(T_Empty);
	FSlateBrush NormalBrush = CreateSlateBrushFromTexture(T_Color1_Normal);
	FSlateBrush PressedBrush = CreateSlateBrushFromTexture(T_Color1_Pressed);
	btn_Style.SetNormal(EmptyBrush);
	btn_Style.SetHovered(NormalBrush);
	btn_Style.SetPressed(PressedBrush);
	btn_Style.SetDisabled(Characters_Btn_Style.Disabled);
	btn_Style.SetNormalPadding(Characters_Btn_Style.NormalPadding);
	btn_Style.SetPressedPadding(Characters_Btn_Style.PressedPadding);

	Characters_Btn->Button->SetStyle(btn_Style);
	Accessoires_Btn->Button->SetStyle(btn_Style);

	// Sets a button to selected
	FSlateBrush HoveredBrush = CreateSlateBrushFromTexture(T_Color1_Hovered);
	FButtonStyle btn_Style_Selected;
	FButtonStyle btn_Style_Target = Button_Target->GetStyle();

	btn_Style_Selected.SetNormal(HoveredBrush);
	btn_Style_Selected.SetHovered(HoveredBrush);
	btn_Style_Selected.SetPressed(HoveredBrush);
	btn_Style_Selected.SetDisabled(btn_Style_Target.Disabled);
	btn_Style_Selected.SetNormalPadding(btn_Style_Target.NormalPadding);
	btn_Style_Selected.SetPressedPadding(btn_Style_Target.PressedPadding);

	Button_Target->SetStyle(btn_Style_Selected);
}

void UW_Character_Customization::NativeConstruct()
{
	Super::NativeConstruct();

	Switch_ActiveTab(0);

	Load_Available_Characters();

	// Button Event Bind
	if (Characters_Btn)
	{
		Characters_Btn->Button->OnClicked.AddDynamic(this, &UW_Character_Customization::OnCharacters_BtnClicked);
	}

	if (Accessoires_Btn)
	{
		Accessoires_Btn->Button->OnClicked.AddDynamic(this, &UW_Character_Customization::OnAccessoires_BtnClicked);
	}

	if (Close_Btn)
	{
		Close_Btn->Button->OnClicked.AddDynamic(this, &UW_Character_Customization::OnClose_BtnClicked);
	}
}

FSlateBrush UW_Character_Customization::CreateSlateBrushFromTexture(UTexture2D* Texture) const
{
	FSlateBrush Brush;
	if (Texture)
	{
		Brush.SetResourceObject(Texture);
		//Brush.ImageSize = FVector2D(Texture->GetSizeX(), Texture->GetSizeY());
	}
	Brush.DrawAs = ESlateBrushDrawType::Box;
	Brush.Tiling = ESlateBrushTileType::NoTile;
	Brush.Mirroring = ESlateBrushMirrorType::NoMirror;

	return Brush;
}

void UW_Character_Customization::Load_Available_Characters()
{
	AvailableCharacters_Wrapbox->ClearChildren();

	FPermissionListOwners MyOutRowNames;
	UDataTableFunctionLibrary::GetDataTableRowNames(DT_Available_Characters, MyOutRowNames);

	for (FName RowName : MyOutRowNames)
	{
		// WB_Character_Customization_Item 위젯 생성하기
		// 구조체로 캐스팅하여 데이터 가져오기
		S_Available_Characters = *(DT_Available_Characters->FindRow<FStructure_Available_Characters>(RowName, TEXT("")));

		Character_Customization_Item_inst = CreateWidget<UW_Character_Customization_Item>(this, Character_Customization_Item_bp, RowName);

		if (Character_Customization_Item_inst)
		{
			// S_Available_Characters 가 유효하면
			if (!S_Available_Characters.Name.IsEmpty())
			{
				Character_Customization_Item_inst->Character = S_Available_Characters;
			}
			Character_Customization_Item_inst->SetOwningPlayer(GetOwningPlayer());
			Character_Customization_Item_inst->ItemType = "Character";

			// Add Child
			AvailableCharacters_Wrapbox->AddChild(Character_Customization_Item_inst);
		}
	}

	Find_Current_Selected_Character();
}

void UW_Character_Customization::Find_Current_Selected_Character()
{
	APS_Lobby* ps_lobby = Cast<APS_Lobby>(GetOwningPlayer()->PlayerState);
	UW_Character_Customization_Item* WB_Character_Customization_Item = nullptr;
	bool bFound = false;
	
	// if ps_lobby cast failed
	if (!ps_lobby)
	{
		return;
	}

	for (UWidget* child : AvailableCharacters_Wrapbox->GetAllChildren()) {
		WB_Character_Customization_Item = Cast<UW_Character_Customization_Item>(child);
		if (WB_Character_Customization_Item->Character.ItemID == ps_lobby->Player_Appearance.Character.ItemID)
		{
			bFound = true;
			break;
		}
	}
	if (bFound)
	{
		WB_Character_Customization_Item->Checkmark_Icon->SetVisibility(ESlateVisibility::Visible);
		return;
	}
	else
	{
		UW_Character_Customization_Item* item = Cast<UW_Character_Customization_Item>(AvailableCharacters_Wrapbox->GetAllChildren()[0]);
		item->Checkmark_Icon->SetVisibility(ESlateVisibility::Visible);
		return;
	}
	
}

void UW_Character_Customization::OnCharacters_BtnClicked()
{
	Switch_ActiveTab(0);
}

void UW_Character_Customization::OnAccessoires_BtnClicked()
{
	Switch_ActiveTab(1);
}


void UW_Character_Customization::OnClose_BtnClicked()
{
	RemoveFromParent();
	APlayerController* pc = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	pc->SetShowMouseCursor(false);
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(pc);
}
