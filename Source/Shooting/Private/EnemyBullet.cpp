// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBullet.h"
#include "kismet/GameplayStatics.h"
#include "ShootingFlight.h"
#include "Components/BoxComponent.h"
#include "ShootingGameModeBase.h"

AEnemyBullet::AEnemyBullet() {
	
	BoxComponent->SetCollisionProfileName(TEXT("EnemyPreset"));
	BoxComponent->SetGenerateOverlapEvents(true);
}

void AEnemyBullet::BeginPlay() {
	Super::BeginPlay();

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AEnemyBullet::OnOverlap);
}

void AEnemyBullet::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	
	// OtherActor �� ShootingFlight ���� Ȯ��
	AShootingFlight* PlayerFlight = Cast<AShootingFlight>(OtherActor);

	// �浹 ����� �÷��̾���..
	if (PlayerFlight != nullptr)
	{
		// �÷��̾ ����
		PlayerFlight->Destroy();

		// MenuWidget�� �����ϴ� �Լ��� ����
		AShootingGameModeBase* GameMode = Cast<AShootingGameModeBase>(GetWorld()->GetAuthGameMode());
		if (GameMode != nullptr)
		{
			GameMode->ShowMenu();
		}

		// �����ε� ����
		Destroy();
	}
}

void AEnemyBullet::SetDirection(FVector NewDirection) {

	// �̵� ������ NewDirection���� ����
	direction = NewDirection;
}