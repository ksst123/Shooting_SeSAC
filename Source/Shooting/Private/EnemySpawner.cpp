// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"
#include "Enemy.h"
#include "Components/ArrowComponent.h"

// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("Root Scene"));
	RootComponent = RootScene;

	SpawnDirection = CreateDefaultSubobject<UArrowComponent>(TEXT("Spawn Direction"));
	SpawnDirection->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//// ���� IsSpawn�� ���� False ��� Tick �Լ��� ����
	//if (!IsSpawn)
	//{
	//	return;
	//}

	// ����� �ð��� ����
	CurrentTime += DeltaTime;
	// ���� ���� �ð��� SpawnTime �̻��� ���
	if (CurrentTime >= SpawnTime)
	{
		// Enemy �������Ʈ�� ����
		GetWorld()->SpawnActor<AEnemy>(EnemyFactory, SpawnDirection->GetComponentLocation(), SpawnDirection->GetComponentRotation());
		//������ �ð��� �ٽ� 0���� �ʱ�ȭ
		CurrentTime = 0;
	}
}

