// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBullet.h"
#include "kismet/GameplayStatics.h"
#include "Enemy.h"
#include "Components/BoxComponent.h"
#include "ShootingGameModeBase.h"

APlayerBullet::APlayerBullet() {
	
	BoxComponent->SetCollisionProfileName(TEXT("BulletPreset"));
}

void APlayerBullet::BeginPlay() {

	// �θ��� BeginPlay()�� �״�� ����
	Super::BeginPlay();

	SetDirection(GetActorForwardVector());
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &APlayerBullet::OnOverlap);
}

// OtherActor �� ��� ���� ����, HitComponent �� �浹 ��� ������Ʈ ����?
void APlayerBullet::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {

	// ���� �浹 ����� Enemy ���
	AEnemy* Enemy = Cast<AEnemy>(OtherActor);
	if (Enemy != nullptr)
	{
		// ���� �ִ� ��ġ�� ���� ����Ʈ ����
		FVector EnemyLocation = Enemy->GetActorLocation();
		FRotator EnemyRotation = Enemy->GetActorRotation();
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, EnemyLocation, EnemyRotation, true);

		// Enemy ����
		Enemy->Destroy();

		// ���� ��� �ȿ� ���� 1�� �߰�
		AGameModeBase* GameMode = UGameplayStatics::GetGameMode(this);
		// AGameModeBase* GameMode = GetWorld()->GetAuthGameMode();
		AShootingGameModeBase* ShootingGameMode = Cast<AShootingGameModeBase>(GameMode);
		ShootingGameMode->AddScore(10);
		// UE_LOG(LogTemp, Warning, TEXT("Score: %d"), ShootingGameMode->GetCurrentScore());

		// Bullet �����ε� ����
		Destroy();
	}
}

void APlayerBullet::SetDirection(FVector NewDirection) {

	// �̵� ������ NewDirection���� ����
	direction = NewDirection;
}