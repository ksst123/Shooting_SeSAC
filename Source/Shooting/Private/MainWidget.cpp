// Fill out your copyright notice in the Description page of Project Settings.


#include "MainWidget.h"
#include "ShootingGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"

void UMainWidget::PrintCurrentScore() {

	AShootingGameModeBase* MyGameMode = Cast<AShootingGameModeBase>(UGameplayStatics::GetGameMode(this));
	
	if (MyGameMode != nullptr)
	{
		// ���� ����(����) -> FText(���ڿ�) ���·� ��ȯ
		FText ScoreToText = FText::AsNumber(MyGameMode->GetCurrentScore());
		// �ؼ� ScoreText �ؽ�Ʈ ����� ������ ����
		ScoreText->SetText(ScoreToText);
	}
}

void UMainWidget::PrintHighScore() {
	AShootingGameModeBase* MyGameMode = Cast<AShootingGameModeBase>(UGameplayStatics::GetGameMode(this));

	if (MyGameMode != nullptr)
	{
		// �ְ� ����(����) -> FText(���ڿ�) ���·� ��ȯ
		FText HighScoreToText = FText::AsNumber(MyGameMode->GetHighScore());
		// �ؼ� HighScoreText �ؽ�Ʈ ����� ������ ����
		HighScoreText->SetText(HighScoreToText);
	}
}
