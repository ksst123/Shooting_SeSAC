// Fill out your copyright notice in the Description page of Project Settings.


#include "BossEnemy.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ABossEnemy::ABossEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	RootComponent = BoxCollision;
	BoxCollision->SetBoxExtent(FVector(50.0f, 300.0f, 150.0f));
	BoxCollision->SetGenerateOverlapEvents(true);
	BoxCollision->SetCollisionProfileName(TEXT("BossPreset"));

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetRelativeScale3D(FVector(1.0f, 6.0f, 3.0f));
	MeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, -150.0f));
	MeshComponent->SetGenerateOverlapEvents(false);

}

// Called when the game starts or when spawned
void ABossEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	StartLocation = GetActorLocation();
	TargetLocation = FVector(0.0f, 0.0f, 200.0f);
}

// Called every frame
void ABossEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//if (GetActorLocation().Z >= 200.0f)
	//{
	//	// 현재 위치에서 (0.0f, 0.0f, 200.0f) 위치까지 이동
	//	SetActorLocation(GetActorLocation() - FVector(0.0f, 0.0f, DeltaTime * 100));


	//}

	// 현재 위치에서 (0.0f, 0.0f, 200.0f) 위치까지 이동
	//float distance = (StartLocation - GetActorLocation()).Length();
	//float shaking = FMath::RandRange(-1.0f, 1.0f);
	//if (distance <= 500.0f)
	//{
	//	FVector direction = GetActorUpVector() * -1;
	//	direction.Y = shaking;
	//	SetActorLocation(GetActorLocation() + direction * 200.0f * DeltaTime);
	//}
	//else
	//{
	//	// 원하는 위치에서 오차가 생길 경우 수정
	//	SetActorLocation(FVector(0.0f, 0.0f, 200.0f));
	//}


	// 현재 위치에서 (0.0f, 0.0f, 200.0f) 위치까지 이동(선형 보간)
	TestValue += DeltaTime;
	
	// TestValue가 어떤 범위 안에서 벗어나지 못하도록(Clamp) 한다
	TestValue = FMath::Clamp(TestValue, 0.0f, 1.0f);

	FVector Result = FMath::Lerp(StartLocation, TargetLocation, TestValue);
	SetActorLocation(Result);
}
	

