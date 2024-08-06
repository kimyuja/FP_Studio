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

	// DT_Available_Accessories 를 로드한다.
	static ConstructorHelpers::FObjectFinder<UDataTable> InGameAvailableAccessoriesTable(TEXT("/Game/KYJ/Data/DT_Available_Accessories"));
	if (InGameAvailableAccessoriesTable.Succeeded())
	{
		DT_Available_Accessories = InGameAvailableAccessoriesTable.Object;
	}

	// DT_Available_Skins 를 로드한다.
	static ConstructorHelpers::FObjectFinder<UDataTable> InGameAvailableSkinsTable(TEXT("/Game/KYJ/Data/DT_Available_Skins2"));
	if (InGameAvailableSkinsTable.Succeeded())
	{
		DT_Available_Skins = InGameAvailableSkinsTable.Object;
	}

	// DT_Available_Eyes 를 로드한다.
	static ConstructorHelpers::FObjectFinder<UDataTable> InGameAvailableEyesTable(TEXT("/Game/KYJ/Data/DT_Available_Eyes"));
	if (InGameAvailableEyesTable.Succeeded())
	{
		DT_Available_Eyes = InGameAvailableEyesTable.Object;
	}

	// DT_Available_Top 를 로드한다.
	static ConstructorHelpers::FObjectFinder<UDataTable> InGameAvailableTopTable(TEXT("/Game/KYJ/Data/DT_Available_Top"));
	if (InGameAvailableTopTable.Succeeded())
	{
		DT_Available_Top = InGameAvailableTopTable.Object;
	}

	// DT_Available_Bottom 를 로드한다.
	static ConstructorHelpers::FObjectFinder<UDataTable> InGameAvailableBottomTable(TEXT("/Game/KYJ/Data/DT_Available_Bottom"));
	if (InGameAvailableBottomTable.Succeeded())
	{
		DT_Available_Bottom = InGameAvailableBottomTable.Object;
	}

	// DT_Available_Outer 를 로드한다.
	static ConstructorHelpers::FObjectFinder<UDataTable> InGameAvailableOuterTable(TEXT("/Game/KYJ/Data/DT_Available_Outer"));
	if (InGameAvailableOuterTable.Succeeded())
	{
		DT_Available_Outer = InGameAvailableOuterTable.Object;
	}

	// DT_Available_Dress 를 로드한다.
	static ConstructorHelpers::FObjectFinder<UDataTable> InGameAvailableDressTable(TEXT("/Game/KYJ/Data/DT_Available_Dress"));
	if (InGameAvailableDressTable.Succeeded())
	{
		DT_Available_Dress = InGameAvailableDressTable.Object;
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
	case 2:
		Set_ActiveTab(Skins_Btn->Button);
		break;
	case 3:
		Set_ActiveTab(Clothes_Btn->Button);
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
	Skins_Btn->Button->SetStyle(btn_Style);
	Clothes_Btn->Button->SetStyle(btn_Style);

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

void UW_Character_Customization::Refresh_Widget()
{
	Load_Available_Characters();
	Load_Available_Accessories();
	Load_Available_Skins();
	Load_Available_Eyes();
	Load_Available_Top();
	Load_Available_Bottom();
	Load_Available_Outer();
	Load_Available_Dress();
}

void UW_Character_Customization::NativeConstruct()
{
	Super::NativeConstruct();

	PS_Lobby = Cast<APS_Lobby>(GetOwningPlayer()->PlayerState);

	Switch_ActiveTab(0);

	Load_Available_Characters();
	Load_Available_Accessories();
	Load_Available_Skins();
	Load_Available_Eyes();
	Load_Available_Top();
	Load_Available_Bottom();
	Load_Available_Outer();
	Load_Available_Dress();

	// Button Event Bind
	if (Characters_Btn)
	{
		Characters_Btn->Button->OnClicked.AddDynamic(this, &UW_Character_Customization::OnCharacters_BtnClicked);
	}

	if (Accessoires_Btn)
	{
		Accessoires_Btn->Button->OnClicked.AddDynamic(this, &UW_Character_Customization::OnAccessoires_BtnClicked);
	}

	if (Skins_Btn)
	{
		Skins_Btn->Button->OnClicked.AddDynamic(this, &UW_Character_Customization::OnSkins_BtnClicked);
	}

	if (Clothes_Btn)
	{
		Clothes_Btn->Button->OnClicked.AddDynamic(this, &UW_Character_Customization::OnClothes_BtnClicked);
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

	TArray<FName> MyOutRowNames;
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

void UW_Character_Customization::Load_Available_Accessories()
{
	AvailableAccessoires_Wrapbox->ClearChildren();

	TArray<FName> MyOutRowNames;
	UDataTableFunctionLibrary::GetDataTableRowNames(DT_Available_Accessories, MyOutRowNames);

	for (FName RowName : MyOutRowNames)
	{
		// WB_Character_Customization_Item 위젯 생성하기
		// 구조체로 캐스팅하여 데이터 가져오기
		S_Available_Accessories = *(DT_Available_Accessories->FindRow<FStructure_Available_Accessories>(RowName, TEXT("")));


		if (PS_Lobby == nullptr) {
			UE_LOG(LogTemp, Error, TEXT("PS_Lobby is null"));
		}
		else if (!PS_Lobby->Player_Appearance.Character.Mesh->IsValidLowLevel()) {
			UE_LOG(LogTemp, Error, TEXT("Player_Appearance.Character is not valid."));
		}
		else {
			UE_LOG(LogTemp, Log, TEXT("PS_Lobby and Player_Appearance.Character are valid"));
			// S_Available_Accessories.CharacterID == -1 이면 모든 캐릭터에 할당 가능한 아이템이라는 뜻
			// 현재 나의 캐릭터에 맞는 아이템만 AddChild 하고 싶다...
			if (S_Available_Accessories.CharacterID != -1 && S_Available_Accessories.CharacterID != PS_Lobby->Player_Appearance.Character.ItemID) {
				// 현재 나의 캐릭터에 맞지 않는 아이템이라면 그냥 넘어가!
				continue;
			}
		}


		Character_Customization_Item_inst = CreateWidget<UW_Character_Customization_Item>(this, Character_Customization_Item_bp, RowName);

		if (Character_Customization_Item_inst)
		{
			// S_Available_Accessories 가 유효하면
			if (!S_Available_Accessories.Name.IsEmpty())
			{
				Character_Customization_Item_inst->Accessories = S_Available_Accessories;
			}
			Character_Customization_Item_inst->SetOwningPlayer(GetOwningPlayer());
			Character_Customization_Item_inst->ItemType = "Accessories";

			// Add Child
			AvailableAccessoires_Wrapbox->AddChild(Character_Customization_Item_inst);
		}
	}

	Find_Current_Selected_Accessories();
}

void UW_Character_Customization::Load_Available_Skins()
{
	AvailableSkins_Wrapbox->ClearChildren();

	TArray<FName> MyOutRowNames;
	UDataTableFunctionLibrary::GetDataTableRowNames(DT_Available_Skins, MyOutRowNames);

	for (FName RowName : MyOutRowNames)
	{
		// WB_Character_Customization_Item 위젯 생성하기
		// 구조체로 캐스팅하여 데이터 가져오기
		S_Available_Skins = *(DT_Available_Skins->FindRow<FStructure_Available_Skins>(RowName, TEXT("")));

		// S_Available_Skins.CharacterID == -1 이면 모든 캐릭터에 할당 가능한 아이템이라는 뜻
		// 현재 나의 캐릭터에 맞는 아이템만 AddChild 하고 싶다...
		if (S_Available_Skins.CharacterID != -1 && S_Available_Skins.CharacterID != PS_Lobby->Player_Appearance.Character.ItemID)
		{
			// 현재 나의 캐릭터에 맞지 않는 아이템이라면 그냥 넘어가!
			continue;
		}

		Character_Customization_Item_inst = CreateWidget<UW_Character_Customization_Item>(this, Character_Customization_Item_bp, RowName);

		if (Character_Customization_Item_inst)
		{
			// S_Available_Skins 가 유효하면
			if (!S_Available_Skins.Name.IsEmpty())
			{
				Character_Customization_Item_inst->Skins = S_Available_Skins;
			}
			Character_Customization_Item_inst->SetOwningPlayer(GetOwningPlayer());
			Character_Customization_Item_inst->ItemType = "Skins";

			// Add Child
			AvailableSkins_Wrapbox->AddChild(Character_Customization_Item_inst);
		}
	}

	Find_Current_Selected_Skins();
}

void UW_Character_Customization::Load_Available_Eyes()
{
	TArray<FName> MyOutRowNames;
	UDataTableFunctionLibrary::GetDataTableRowNames(DT_Available_Eyes, MyOutRowNames);

	for (FName RowName : MyOutRowNames)
	{
		// WB_Character_Customization_Item 위젯 생성하기
		// 구조체로 캐스팅하여 데이터 가져오기
		S_Available_Eyes = *(DT_Available_Eyes->FindRow<FStructure_Available_Eyes>(RowName, TEXT("")));

		// S_Available_Eyes.CharacterID == -1 이면 모든 캐릭터에 할당 가능한 아이템이라는 뜻
		// 현재 나의 캐릭터에 맞는 아이템만 AddChild 하고 싶다...
		if (S_Available_Eyes.CharacterID != -1 && S_Available_Eyes.CharacterID != PS_Lobby->Player_Appearance.Character.ItemID)
		{
			// 현재 나의 캐릭터에 맞지 않는 아이템이라면 그냥 넘어가!
			continue;
		}

		Character_Customization_Item_inst = CreateWidget<UW_Character_Customization_Item>(this, Character_Customization_Item_bp, RowName);

		if (Character_Customization_Item_inst)
		{
			// S_Available_Eyes 가 유효하면
			if (!S_Available_Eyes.Name.IsEmpty())
			{
				Character_Customization_Item_inst->Eyes = S_Available_Eyes;
			}
			Character_Customization_Item_inst->SetOwningPlayer(GetOwningPlayer());
			Character_Customization_Item_inst->ItemType = "Eyes";

			// Add Child
			AvailableSkins_Wrapbox->AddChild(Character_Customization_Item_inst);
		}
	}

	Find_Current_Selected_Eyes();
}

void UW_Character_Customization::Load_Available_Top()
{
	AvailableClothes_Wrapbox->ClearChildren();

	TArray<FName> MyOutRowNames;
	UDataTableFunctionLibrary::GetDataTableRowNames(DT_Available_Top, MyOutRowNames);

	for (FName RowName : MyOutRowNames)
	{
		// WB_Character_Customization_Item 위젯 생성하기
		// 구조체로 캐스팅하여 데이터 가져오기
		S_Available_Top = *(DT_Available_Top->FindRow<FStructure_Available_Top>(RowName, TEXT("")));

		// S_Available_Top.CharacterID == -1 이면 모든 캐릭터에 할당 가능한 아이템이라는 뜻
		// 현재 나의 캐릭터에 맞는 아이템만 AddChild 하고 싶다...
		if (S_Available_Top.CharacterID != -1 && S_Available_Top.CharacterID != PS_Lobby->Player_Appearance.Character.ItemID)
		{
			// 현재 나의 캐릭터에 맞지 않는 아이템이라면 그냥 넘어가!
			continue;
		}

		Character_Customization_Item_inst = CreateWidget<UW_Character_Customization_Item>(this, Character_Customization_Item_bp, RowName);

		if (Character_Customization_Item_inst)
		{
			// S_Available_Accessories 가 유효하면
			if (!S_Available_Top.Name.IsEmpty())
			{
				Character_Customization_Item_inst->Top = S_Available_Top;
			}
			Character_Customization_Item_inst->SetOwningPlayer(GetOwningPlayer());
			Character_Customization_Item_inst->ItemType = "Top";

			// Add Child
			AvailableClothes_Wrapbox->AddChild(Character_Customization_Item_inst);
		}
	}

	Find_Current_Selected_Top();
}

void UW_Character_Customization::Load_Available_Bottom()
{
	TArray<FName> MyOutRowNames;
	UDataTableFunctionLibrary::GetDataTableRowNames(DT_Available_Bottom, MyOutRowNames);

	for (FName RowName : MyOutRowNames)
	{
		// WB_Character_Customization_Item 위젯 생성하기
		// 구조체로 캐스팅하여 데이터 가져오기
		S_Available_Bottom = *(DT_Available_Bottom->FindRow<FStructure_Available_Bottom>(RowName, TEXT("")));

		// S_Available_Top.CharacterID == -1 이면 모든 캐릭터에 할당 가능한 아이템이라는 뜻
		// 현재 나의 캐릭터에 맞는 아이템만 AddChild 하고 싶다...
		if (S_Available_Bottom.CharacterID != -1 && S_Available_Bottom.CharacterID != PS_Lobby->Player_Appearance.Character.ItemID)
		{
			// 현재 나의 캐릭터에 맞지 않는 아이템이라면 그냥 넘어가!
			continue;
		}

		Character_Customization_Item_inst = CreateWidget<UW_Character_Customization_Item>(this, Character_Customization_Item_bp, RowName);

		if (Character_Customization_Item_inst)
		{
			// S_Available_Accessories 가 유효하면
			if (!S_Available_Bottom.Name.IsEmpty())
			{
				Character_Customization_Item_inst->Bottom = S_Available_Bottom;
			}
			Character_Customization_Item_inst->SetOwningPlayer(GetOwningPlayer());
			Character_Customization_Item_inst->ItemType = "Bottom";

			// Add Child
			AvailableClothes_Wrapbox->AddChild(Character_Customization_Item_inst);
		}
	}

	Find_Current_Selected_Bottom();
}

void UW_Character_Customization::Load_Available_Outer()
{
	TArray<FName> MyOutRowNames;
	UDataTableFunctionLibrary::GetDataTableRowNames(DT_Available_Outer, MyOutRowNames);

	for (FName RowName : MyOutRowNames)
	{
		// WB_Character_Customization_Item 위젯 생성하기
		// 구조체로 캐스팅하여 데이터 가져오기
		S_Available_Outer = *(DT_Available_Outer->FindRow<FStructure_Available_Outer>(RowName, TEXT("")));

		// S_Available_Outer.CharacterID == -1 이면 모든 캐릭터에 할당 가능한 아이템이라는 뜻
		// 현재 나의 캐릭터에 맞는 아이템만 AddChild 하고 싶다...
		if (S_Available_Outer.CharacterID != -1 && S_Available_Outer.CharacterID != PS_Lobby->Player_Appearance.Character.ItemID)
		{
			// 현재 나의 캐릭터에 맞지 않는 아이템이라면 그냥 넘어가!
			continue;
		}

		Character_Customization_Item_inst = CreateWidget<UW_Character_Customization_Item>(this, Character_Customization_Item_bp, RowName);

		if (Character_Customization_Item_inst)
		{
			// S_Available_Accessories 가 유효하면
			if (!S_Available_Outer.Name.IsEmpty())
			{
				Character_Customization_Item_inst->Outer = S_Available_Outer;
			}
			Character_Customization_Item_inst->SetOwningPlayer(GetOwningPlayer());
			Character_Customization_Item_inst->ItemType = "Outer";

			// Add Child
			AvailableClothes_Wrapbox->AddChild(Character_Customization_Item_inst);
		}
	}

	Find_Current_Selected_Outer();
}

void UW_Character_Customization::Load_Available_Dress()
{
	TArray<FName> MyOutRowNames;
	UDataTableFunctionLibrary::GetDataTableRowNames(DT_Available_Dress, MyOutRowNames);

	for (FName RowName : MyOutRowNames)
	{
		// WB_Character_Customization_Item 위젯 생성하기
		// 구조체로 캐스팅하여 데이터 가져오기
		S_Available_Dress = *(DT_Available_Dress->FindRow<FStructure_Available_Dress>(RowName, TEXT("")));

		// S_Available_Dress.CharacterID == -1 이면 모든 캐릭터에 할당 가능한 아이템이라는 뜻
		// 현재 나의 캐릭터에 맞는 아이템만 AddChild 하고 싶다...
		if (S_Available_Dress.CharacterID != -1 && S_Available_Dress.CharacterID != PS_Lobby->Player_Appearance.Character.ItemID)
		{
			// 현재 나의 캐릭터에 맞지 않는 아이템이라면 그냥 넘어가!
			continue;
		}

		Character_Customization_Item_inst = CreateWidget<UW_Character_Customization_Item>(this, Character_Customization_Item_bp, RowName);

		if (Character_Customization_Item_inst)
		{
			// S_Available_Accessories 가 유효하면
			if (!S_Available_Dress.Name.IsEmpty())
			{
				Character_Customization_Item_inst->Dress = S_Available_Dress;
			}
			Character_Customization_Item_inst->SetOwningPlayer(GetOwningPlayer());
			Character_Customization_Item_inst->ItemType = "Dress";

			// Add Child
			AvailableClothes_Wrapbox->AddChild(Character_Customization_Item_inst);
		}
	}

	Find_Current_Selected_Dress();
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
			if (ps_lobby->Player_Appearance.Character.ItemID == 2 && WB_Character_Customization_Item->Character.Name != ps_lobby->Player_Appearance.Character.Name)
			{
				// turtle no shell / with shell
				continue;
			}
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

void UW_Character_Customization::Find_Current_Selected_Accessories()
{
	APS_Lobby* ps_lobby = Cast<APS_Lobby>(GetOwningPlayer()->PlayerState);

	UW_Character_Customization_Item* WB_Character_Customization_Item = nullptr;
	bool bFound = false;

	// if ps_lobby cast failed
	if (!ps_lobby)
	{
		return;
	}

	for (UWidget* child : AvailableAccessoires_Wrapbox->GetAllChildren()) {
		WB_Character_Customization_Item = Cast<UW_Character_Customization_Item>(child);
		if (WB_Character_Customization_Item->Accessories.ItemID == ps_lobby->Player_Appearance.Accessories_Slot.ItemID)
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
		UW_Character_Customization_Item* item = Cast<UW_Character_Customization_Item>(AvailableAccessoires_Wrapbox->GetAllChildren()[0]);
		item->Checkmark_Icon->SetVisibility(ESlateVisibility::Visible);
		return;
	}
}

void UW_Character_Customization::Find_Current_Selected_Skins()
{
	APS_Lobby* ps_lobby = Cast<APS_Lobby>(GetOwningPlayer()->PlayerState);

	UW_Character_Customization_Item* WB_Character_Customization_Item = nullptr;
	bool bFound = false;

	// if ps_lobby cast failed
	if (!ps_lobby)
	{
		return;
	}

	// Eyes 첫 인덱스 찾아내기 위해서 Skins 의 개수를 저장해놔야 함
	TArray<UWidget*> allChildren = AvailableSkins_Wrapbox->GetAllChildren();
	SkinsNum = allChildren.Num();

	for (UWidget* child : allChildren) {
		WB_Character_Customization_Item = Cast<UW_Character_Customization_Item>(child);
		if (WB_Character_Customization_Item->Skins.ItemID == ps_lobby->Player_Appearance.Skins_Slot.ItemID)
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
		UW_Character_Customization_Item* item = Cast<UW_Character_Customization_Item>(AvailableSkins_Wrapbox->GetAllChildren()[0]);
		item->Checkmark_Icon->SetVisibility(ESlateVisibility::Visible);
		item->OnClickedButton();
		return;
	}
}

void UW_Character_Customization::Find_Current_Selected_Eyes()
{
	APS_Lobby* ps_lobby = Cast<APS_Lobby>(GetOwningPlayer()->PlayerState);

	UW_Character_Customization_Item* WB_Character_Customization_Item = nullptr;
	bool bFound = false;

	// if ps_lobby cast failed
	if (!ps_lobby)
	{
		return;
	}

	for (UWidget* child : AvailableSkins_Wrapbox->GetAllChildren()) {
		WB_Character_Customization_Item = Cast<UW_Character_Customization_Item>(child);
		if ((WB_Character_Customization_Item->ItemType == "Eyes") && WB_Character_Customization_Item->Eyes.ItemID == ps_lobby->Player_Appearance.Eyes_Slot.ItemID)
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
		UW_Character_Customization_Item* item = Cast<UW_Character_Customization_Item>(AvailableSkins_Wrapbox->GetAllChildren()[SkinsNum]);
		item->Checkmark_Icon->SetVisibility(ESlateVisibility::Visible);
		item->OnClickedButton();
		return;
	}
}

void UW_Character_Customization::Find_Current_Selected_Top()
{
	APS_Lobby* ps_lobby = Cast<APS_Lobby>(GetOwningPlayer()->PlayerState);

	UW_Character_Customization_Item* WB_Character_Customization_Item = nullptr;
	bool bFound = false;

	// if ps_lobby cast failed
	if (!ps_lobby)
	{
		return;
	}

	// Bottom 첫 인덱스 찾아내기 위해서 Top 의 개수를 저장해놔야 함
	TArray<UWidget*> allChildren = AvailableClothes_Wrapbox->GetAllChildren();
	TopNum = allChildren.Num();

	for (UWidget* child : allChildren) {
		WB_Character_Customization_Item = Cast<UW_Character_Customization_Item>(child);
		if (WB_Character_Customization_Item->Top.ItemID == ps_lobby->Player_Appearance.Top_Slot.ItemID)
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
		UW_Character_Customization_Item* item = Cast<UW_Character_Customization_Item>(AvailableClothes_Wrapbox->GetAllChildren()[0]);
		item->Checkmark_Icon->SetVisibility(ESlateVisibility::Visible);
		item->OnClickedButton();
		return;
	}
}

void UW_Character_Customization::Find_Current_Selected_Bottom()
{
	APS_Lobby* ps_lobby = Cast<APS_Lobby>(GetOwningPlayer()->PlayerState);

	UW_Character_Customization_Item* WB_Character_Customization_Item = nullptr;
	bool bFound = false;

	// if ps_lobby cast failed
	if (!ps_lobby)
	{
		return;
	}

	// Outer 첫 인덱스 찾아내기 위해서 Top + Bottom 의 개수를 저장해놔야 함
	TArray<UWidget*> allChildren = AvailableClothes_Wrapbox->GetAllChildren();
	TopPlusBottomNum = allChildren.Num();

	for (UWidget* child : allChildren) {
		WB_Character_Customization_Item = Cast<UW_Character_Customization_Item>(child);
		if (WB_Character_Customization_Item->Bottom.ItemID == ps_lobby->Player_Appearance.Bottom_Slot.ItemID)
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
		UW_Character_Customization_Item* item = Cast<UW_Character_Customization_Item>(AvailableClothes_Wrapbox->GetAllChildren()[TopNum]);
		item->Checkmark_Icon->SetVisibility(ESlateVisibility::Visible);
		item->OnClickedButton();
		return;
	}
}

void UW_Character_Customization::Find_Current_Selected_Outer()
{
	APS_Lobby* ps_lobby = Cast<APS_Lobby>(GetOwningPlayer()->PlayerState);

	UW_Character_Customization_Item* WB_Character_Customization_Item = nullptr;
	bool bFound = false;

	// if ps_lobby cast failed
	if (!ps_lobby)
	{
		return;
	}

	// Dress 첫 인덱스 찾아내기 위해서 Top + Bottom + Outer 의 개수를 저장해놔야 함
	TArray<UWidget*> allChildren = AvailableClothes_Wrapbox->GetAllChildren();
	TopPlusBottomPlusOuterNum = allChildren.Num();

	for (UWidget* child : allChildren) {
		WB_Character_Customization_Item = Cast<UW_Character_Customization_Item>(child);
		if (WB_Character_Customization_Item->Outer.ItemID == ps_lobby->Player_Appearance.Outer_Slot.ItemID)
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
		UW_Character_Customization_Item* item = Cast<UW_Character_Customization_Item>(AvailableClothes_Wrapbox->GetAllChildren()[TopPlusBottomNum]);
		item->Checkmark_Icon->SetVisibility(ESlateVisibility::Visible);
		item->OnClickedButton();
		return;
	}
}

void UW_Character_Customization::Find_Current_Selected_Dress()
{
	APS_Lobby* ps_lobby = Cast<APS_Lobby>(GetOwningPlayer()->PlayerState);

	UW_Character_Customization_Item* WB_Character_Customization_Item = nullptr;
	bool bFound = false;

	// if ps_lobby cast failed
	if (!ps_lobby)
	{
		return;
	}

	TArray<UWidget*> allChildren = AvailableClothes_Wrapbox->GetAllChildren();

	for (UWidget* child : allChildren) {
		WB_Character_Customization_Item = Cast<UW_Character_Customization_Item>(child);
		if (WB_Character_Customization_Item->Dress.ItemID == ps_lobby->Player_Appearance.Dress_Slot.ItemID)
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
		UW_Character_Customization_Item* item = Cast<UW_Character_Customization_Item>(AvailableClothes_Wrapbox->GetAllChildren()[TopPlusBottomPlusOuterNum]);
		item->Checkmark_Icon->SetVisibility(ESlateVisibility::Visible);
		item->OnClickedButton();
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

void UW_Character_Customization::OnSkins_BtnClicked()
{
	Switch_ActiveTab(2);
}

void UW_Character_Customization::OnClothes_BtnClicked()
{
	Switch_ActiveTab(3);
}


void UW_Character_Customization::OnClose_BtnClicked()
{
	RemoveFromParent();
	APlayerController* pc = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	pc->SetShowMouseCursor(false);
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(pc);
}
