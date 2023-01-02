// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuWidget.h"
#include "kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "kismet/KismetSystemLibrary.h"

void UMenuWidget::NativeConstruct() {
	
	ButtonResume->OnClicked.AddDynamic(this, &UMenuWidget::ResumeGame);
	ButtonRestart->OnClicked.AddDynamic(this, &UMenuWidget::RestartGame);
	ButtonQuit->OnClicked.AddDynamic(this, &UMenuWidget::QuitGame);
}

void UMenuWidget::ResumeGame() {

	// ���� �Ͻ����� ����
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	// ���콺 Ŀ�� ȭ�� ��� ����
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
	// AddToViewport�� �ݴ� �Լ�
	this->RemoveFromParent();
}

void UMenuWidget::RestartGame() {

	// ������ �ٽ� �ε�
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("LV_ShootingMap"));

	// ���� ������ ����
	this->RemoveFromParent();

	// ���콺 Ŀ�� ȭ�� ��� ����
	// GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);


}

void UMenuWidget::QuitGame() {

	// ���� ����
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	UKismetSystemLibrary::QuitGame(GetWorld(), PlayerController, EQuitPreference::Quit, true);
}
