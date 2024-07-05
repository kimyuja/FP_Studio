#include "W_Base_Button.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "SlateCore.h"
#include "Styling/SlateColor.h"
#include "Engine/Texture2D.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/ButtonSlot.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/SizeBox.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Engine/Font.h>

UW_Base_Button::UW_Base_Button(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    FontObject = Cast<UFont>(StaticLoadObject(UFont::StaticClass(), nullptr, TEXT("/Engine/EngineFonts/Roboto.Roboto")));
}

void UW_Base_Button::Setup()
{
	Set_ButtonStyle(NormalImage, HoveredImage, PressedImage, FontColor);
    Set_ButtonSize();
    Set_Font();
}

void UW_Base_Button::Set_ButtonStyle(UTexture2D* NewNormalImage, UTexture2D* NewHoveredImage, UTexture2D* NewPressedImage, FLinearColor NewFontColor)
{
    // �̹����� SlateBrush �����
    FSlateBrush NormalBrush = CreateSlateBrushFromTexture(NewNormalImage);
    FSlateBrush HoveredBrush = CreateSlateBrushFromTexture(NewHoveredImage);
    FSlateBrush PressedBrush = CreateSlateBrushFromTexture(NewPressedImage);

    // ���� SlateBrush�� ��ư ��Ÿ�� �����ϱ�
    FButtonStyle NewStyle = CreateButtonStyle(NormalBrush, HoveredBrush, PressedBrush);
    Button->SetStyle(NewStyle);

    // TextBlock�� font color �����ϱ�
    SetTextBlockColor(NewFontColor);

    UButtonSlot* ButtonSlot = Cast<UButtonSlot>(Button->GetContentSlot());
    ButtonSlot->SetHorizontalAlignment(Btn_HorizontalAlignment);
    ButtonSlot->SetPadding(Btn_Padding);
    
}

void UW_Base_Button::Set_ButtonSize()
{
    SizeBox->SetWidthOverride(Button_Width);
    SizeBox->SetHeightOverride(Button_Height);
}

FSlateBrush UW_Base_Button::Get_SlateBrush(UTexture2D* Img)
{
    FSlateBrush Brush;
    if (Img)
    {
        Brush.SetResourceObject(Img);
        Brush.ImageSize = FVector2D(Img->GetSizeX(), Img->GetSizeY());
    }
    return Brush;
}

void UW_Base_Button::Set_Font()
{
    // ��ο��� ��Ʈ �ڻ��� �ε��Ͽ� FontObject�� ������ (NativeConstruct����)

    if (FontObject)
    {
        // FSlateFontInfo ��ü�� �����ϰ� ��Ʈ ��ü�� �Ҵ��մϴ�
        FSlateFontInfo NewSlateFontInfo;
        NewSlateFontInfo.FontObject = FontObject;
        NewSlateFontInfo.Size = Font_Size;
        NewSlateFontInfo.TypefaceFontName = FName("Regular");

        // TextBlock�� ��ȿ�� ��� ��Ʈ�� �����մϴ�
        if (TextBlock)
        {
            TextBlock->SetFont(NewSlateFontInfo);
            // ��ư �ؽ�Ʈ�� �����մϴ�
            TextBlock->SetText(Button_Text);
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to load font."));
    }
}

void UW_Base_Button::OnButtonClicked()
{
    // ��ư�� Ŭ���Ǹ� delegate ����
    OnMyButtonClicked.Broadcast();
}

void UW_Base_Button::NativePreConstruct()
{
    Super::NativePreConstruct();
    
    Setup();
}

void UW_Base_Button::NativeConstruct()
{
    Super::NativeConstruct();

    Setup();

    if (Button)
    {
        Button->OnClicked.AddDynamic(this, &UW_Base_Button::OnButtonClicked);
    }
}

FSlateBrush UW_Base_Button::CreateSlateBrushFromTexture(UTexture2D* Texture) const
{
    FSlateBrush Brush;
    if (Texture)
    {
        Brush.SetResourceObject(Texture);
        Brush.ImageSize = FVector2D(Texture->GetSizeX(), Texture->GetSizeY());
    }
    return Brush;
}

FButtonStyle UW_Base_Button::CreateButtonStyle(const FSlateBrush& NormalBrush, const FSlateBrush& HoveredBrush, const FSlateBrush& PressedBrush) const
{
    FButtonStyle ButtonStyle;

    // ��ư ��Ÿ�Ͽ� SlateBrush�� �����Ѵ�.
    ButtonStyle.SetNormal(NormalBrush);
    ButtonStyle.SetHovered(HoveredBrush);
    ButtonStyle.SetPressed(PressedBrush);

    // Break Button Style ���
    ButtonStyle.Disabled = Button->GetStyle().Disabled;
    ButtonStyle.NormalPadding = Button->GetStyle().NormalPadding;
    ButtonStyle.PressedPadding = Button->GetStyle().PressedPadding;

    return ButtonStyle;
}

void UW_Base_Button::SetTextBlockColor(FLinearColor NewFontColor)
{
    TextBlock->SetColorAndOpacity(FSlateColor(NewFontColor));
}
