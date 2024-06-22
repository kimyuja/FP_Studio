#include "W_Base_Button.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "SlateCore.h"
#include "Styling/SlateColor.h"
#include "Engine/Texture2D.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/ButtonSlot.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/SizeBox.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Engine/Font.h>

void UW_Base_Button::Setup()
{
	Set_ButtonStyle(NormalImage, HoveredImage, PressedImage, FontColor);
    Set_ButtonSize();
    Set_Font();
}

void UW_Base_Button::Set_ButtonStyle(UTexture2D* NewNormalImage, UTexture2D* NewHoveredImage, UTexture2D* NewPressedImage, FLinearColor NewFontColor)
{
    // 이미지로 SlateBrush 만들기
    FSlateBrush NormalBrush = CreateSlateBrushFromTexture(NewNormalImage);
    FSlateBrush HoveredBrush = CreateSlateBrushFromTexture(NewHoveredImage);
    FSlateBrush PressedBrush = CreateSlateBrushFromTexture(NewPressedImage);

    // 만든 SlateBrush로 버튼 스타일 지정하기
    FButtonStyle NewStyle = CreateButtonStyle(NormalBrush, HoveredBrush, PressedBrush);
    Button->SetStyle(NewStyle);

    // TextBlock의 font color 지정하기
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
    // 경로에서 폰트 자산을 로드하여 FontObject에 저장함 (NativeConstruct에서)

    if (FontObject)
    {
        // FSlateFontInfo 객체를 생성하고 폰트 객체를 할당합니다
        FSlateFontInfo NewSlateFontInfo;
        NewSlateFontInfo.FontObject = FontObject;
        NewSlateFontInfo.Size = Font_Size;
        NewSlateFontInfo.TypefaceFontName = FName("Regular");

        // TextBlock이 유효한 경우 폰트를 설정합니다
        if (TextBlock)
        {
            TextBlock->SetFont(NewSlateFontInfo);
            // 버튼 텍스트를 설정합니다
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
    // 버튼이 클릭되면 delegate 실행
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

    FontObject = Cast<UFont>(StaticLoadObject(UFont::StaticClass(), nullptr, TEXT("/Engine/EngineFonts/Roboto.Roboto")));

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

    // 버튼 스타일에 SlateBrush를 지정한다.
    ButtonStyle.SetNormal(NormalBrush);
    ButtonStyle.SetHovered(HoveredBrush);
    ButtonStyle.SetPressed(PressedBrush);

    // Break Button Style 노드
    ButtonStyle.Disabled = Button->GetStyle().Disabled;
    ButtonStyle.NormalPadding = Button->GetStyle().NormalPadding;
    ButtonStyle.PressedPadding = Button->GetStyle().PressedPadding;

    return ButtonStyle;
}

void UW_Base_Button::SetTextBlockColor(FLinearColor NewFontColor)
{
    TextBlock->SetColorAndOpacity(FSlateColor(NewFontColor));
}
