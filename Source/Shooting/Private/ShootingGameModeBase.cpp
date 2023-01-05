// Copyright Epic Games, Inc. All Rights Reserved.


#include "ShootingGameModeBase.h"
#include "MainWidget.h"
#include "MenuWidget.h"
#include "kismet/GameplayStatics.h"
#include "BossEnemy.h"
#include "EngineUtils.h"
#include "EnemySpawner.h"

void AShootingGameModeBase::BeginPlay() {
	
	// ���� �������Ʈ�� ����
	MainUI = CreateWidget<UMainWidget>(GetWorld(), MainWidget);

	if (MainUI != nullptr)
	{
		// ������ ���� �������Ʈ�� ����Ʈ�� �׸���.
		MainUI->AddToViewport();
		// ���� ������ ������ ScoreText �ؽ�Ʈ ��Ͽ� ���
		MainUI->PrintCurrentScore();
		// �ְ� ������ ������ HighScoreText �ؽ�Ʈ ��Ͽ� ���
		MainUI->PrintHighScore();

		// �ְ� ������ ���Ͽ��� �о�´�.
		FString LoadScoreText;
		bool isLoadSuccess = FFileHelper::LoadFileToString(LoadScoreText, *SavePath);
		if (isLoadSuccess)
		{
			UE_LOG(LogTemp, Warning, TEXT("Success"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed"));
		}
		//�о�� ���� HighScore ������ ����
		HighScore = FCString::Atoi(*LoadScoreText);
		MainUI->PrintHighScore();
	}

	// FString TestPath = FPaths::GetProjectFilePath();
	// FString TestPath = FPaths::EngineContentDir();
	// FString TestPath = FPaths::GetGameLocalizationPaths();
	// FString TextPath = FPaths::Combine("http://www.naver.com", "akmfldv/asdklmv");
	FString TestPath = FPaths::ProjectContentDir() + TEXT("SaveScore/HighScore.txt");
	UE_LOG(LogTemp, Warning, TEXT("Test Path : %s"), *TestPath);

	// ��ũ���� Log ���
	GEngine->AddOnScreenDebugMessage(1, 3.0f, FColor::Emerald, TEXT("Log Test"), true);
}

void AShootingGameModeBase::AddScore(int32 point) {
	
	CurrentScore += point;

	// ���� ������ CurrentScore�� HighScore ���� ũ�ٸ�
	if (MainUI != nullptr && CurrentScore > HighScore)
	{
		// HighScore�� CurrentScore�� ����
		HighScore = CurrentScore;
		MainUI->PrintHighScore();
	
		// HighScore�� ���Ϸ� ����
		bool isSaved = FFileHelper::SaveStringToFile(FString::FromInt(HighScore), *SavePath);
		UE_LOG(LogTemp, Warning, TEXT("%s"), isSaved ? TEXT("True") : TEXT("False"));
	}
	
	// ���� ������ 30�� �̻��̰� ������ ���� �������� �ʾ�����
	if (CurrentScore >= 30 && !bIsAppearBoss)
	{
		bIsAppearBoss = true;

		// 4�� �ڿ� ���� ����
		FTimerHandle BossSpawnTimer;
		GetWorld()->GetTimerManager().SetTimer(BossSpawnTimer, this, &AShootingGameModeBase::SpawnBoss, 4, false);

		// ��� ���ʹ� ������ �ߴ�
		StopAllEnemySpawn();
	}

	// ���� ������ ������ ScoreText �ؽ�Ʈ ��Ͽ� �ݿ�
	if (MainUI != nullptr)
	{
		MainUI->PrintCurrentScore();
	}
}

void AShootingGameModeBase::ShowMenu() {

	// �޴� ������ ȭ�鿡 ���
	MenuUI = CreateWidget<UMenuWidget>(GetWorld(), MenuWidget);

	if (MenuUI != nullptr)
	{
		MenuUI->AddToViewport();
	}

	// ���� �Ͻ�����
	// 1. ���� ������ �ð� �帧 ������ 0���� �����.
	// UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0);
	// 2. �Ͻ����� �Լ� ȣ��
	UGameplayStatics::SetGamePaused(GetWorld(), true);

	// ���콺 Ŀ�� ȭ�鿡 ���
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
}

void AShootingGameModeBase::SpawnBoss() {

	GetWorld()->SpawnActor<ABossEnemy>(BossSpawner, FVector(0.0f, 0.0f, 700.0f), FRotator::ZeroRotator);
}

void AShootingGameModeBase::StopAllEnemySpawn() {

	for (TActorIterator<AEnemySpawner> spawner(GetWorld()); spawner; ++spawner)
	{
		spawner->IsSpawn = false;
	}
}
