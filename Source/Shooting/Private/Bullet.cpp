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
	BoxComponent->SetCollisionProfileName(TEXT("BulletPreset"));

	// ����ƽ �޽� ����
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	// ����ƽ �޽ø� ��Ʈ ������Ʈ�� �Ҵ�
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetRelativeLocation(FVector(0, 0, -50.0f));
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
	// �浹(overlap) �߻� �� ������ �Լ��� ����
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ABullet::OnOverlap);

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

// OtherActor �� ��� ���� ����, HitComponent �� �浹 ��� ������Ʈ ����?
void ABullet::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	
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
		AGameModeBase* GameMode =  UGameplayStatics::GetGameMode(this);
		// AGameModeBase* GameMode = GetWorld()->GetAuthGameMode();
		AShootingGameModeBase* ShootingGameMode = Cast<AShootingGameModeBase>(GameMode);
		ShootingGameMode->AddScore(10);
		// UE_LOG(LogTemp, Warning, TEXT("Score: %d"), ShootingGameMode->GetCurrentScore());

		// Bullet �����ε� ����
		Destroy();
	}
}

void ABullet::DestroyBulletSelf() {
	Destroy();
}

