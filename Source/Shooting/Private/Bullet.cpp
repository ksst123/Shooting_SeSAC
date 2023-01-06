// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Enemy.h"
#include "Kismet/GameplayStatics.h"
#include "ShootingGameModeBase.h"


// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	// �浹 ���� ����
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	// BoxComponent�� RootComponent �� ����
	RootComponent = BoxComponent;
	BoxComponent->SetBoxExtent(FVector(50.0f));
	BoxComponent->SetWorldScale3D(FVector(0.75f, 0.25f, 1.0f));

	// ����ƽ �޽� ����
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	// ����ƽ �޽ø� ��Ʈ ������Ʈ�� �Ҵ�
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetRelativeLocation(FVector(0, 0, -50.0f));
	MeshComponent->SetGenerateOverlapEvents(false);
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
	// �浹(overlap) �߻� �� ������ �Լ��� ����
	// BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ABullet::OnOverlap);

	// ������ �̺�Ʈ�� �߻��ϵ��� ����
	BoxComponent->SetGenerateOverlapEvents(true);
	
	// ���� �� 3�� �ڿ� ����
	// SetLifeSpan(3.0f);

	// ���� �� 3�� �ڿ� ����, World�� TimerManager �̿�
	GetWorld()->GetTimerManager().SetTimer(TimerHandler, this, &ABullet::DestroyBulletSelf, 3.0f, false);
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ������ ���� �������� ��� �̵�
	// direction = FVector(1, 0, 0);
	direction = GetActorForwardVector();

	FVector dir = GetActorLocation() + direction * MoveSpeed * DeltaTime;
	SetActorLocation(dir);
}



void ABullet::DestroyBulletSelf() {
	Destroy();
}

