# 描画まわりの地図

描画に関わるクラスが「どの層にいて」「誰が何を持ち」「どの順に呼ばれるか」をまとめたもの。
クラスを足したら、この地図のどこに入るかを1行追記する。どこにも入らないなら設計を見直すサイン。

---

## 1. 層

| 層 | クラス | 役割 |
|---|---|---|
| **素材** | `ResourceManager` / `ResourcePool<T>` / `Handle<T>` | ファイルから読み込んだものを共有・寿命管理する |
| | `Texture` / `Mesh` | 素材そのもの。持ち主は ResourceManager |
| **部品**（素材を使って描く） | `Polygon2D` | 画面上の四角形（UI・スプライト・フェード） |
| | `Polygon3D` | 空間上の四角形（ビルボード・パーティクル・空） |
| | `MeshRendererComponent` | 3D モデル |
| | `Sprite2DComponent` / `Sprite3DComponent` / `NumberRenderer` / `SkyPlate` | 上の部品を使う、より上位の描画役 |
| **パイプライン**（フレームの流れを作る） | `RenderTexture` | 描き込み先にも読み込み元にもなるテクスチャ。持ち主が直接持つ |
| | `TransparentRenderManager` | 半透明をカメラから遠い順に並べて描く |
| | `PostEffectManager` / `PostEffect`（`Bloom` `GrayScale` `Posterize`） | シーン画像に効果をかける |
| | `FullscreenQuad` | SRV を画面全体に描く（ポストエフェクト専用） |
| **土台** | `Renderer` | デバイス・コンテキスト・行列・ブレンド・深度など共通設定 |

### テクスチャの2種類

| | 読み込んだテクスチャ | RenderTexture |
|---|---|---|
| 作り方 | `ResourceManager::Load<Texture>` | 自分で `Init`（D3D を直接呼ぶ） |
| 持ち方 | `Handle<Texture>` | 持ち主がメンバーとして持つ |
| 描く部品 | Polygon2D / Polygon3D / MeshRenderer | FullscreenQuad（SRV を Draw の引数で借りる） |
| 寿命 | Scene / Global スコープ | 持ち主と同じ |

---

## 2. 1フレームの流れ（`Manager::Draw`）

```
Renderer::Begin
│
├─ m_RenderTexture を描き込み先にする
│
├─ SceneManager::DrawScene → 各シーンの Draw（例: GameScene）
│    ├─ Camera::Apply / Renderer::Set3D
│    ├─ SkyPlate
│    ├─ GameObjectManager::Draw
│    │    ├─ 通常描画：各オブジェクトの Draw（MeshRenderer など）
│    │    └─ 半透明：Sprite3D・パーティクルを TransparentRenderManager に登録
│    │              → 遠い順に並べて描く（深度書き込みなし、ATC あり）
│    └─ Renderer::Set2D → NumberRenderer（スコア UI）
│
├─ PostEffectManager::Draw(m_RenderTexture の SRV)
│    ├─ 有効なエフェクトを順に：入力 → Ping / Pong に交互に描く
│    └─ 最後の結果を FullscreenQuad でバックバッファへ
│
├─ SceneManager::DrawFade（Polygon2D・ポストエフェクトの後なので効果がかからない）
│
Renderer::End
```

### 気をつけること
- **スコア UI もポストエフェクトを通る**（シーンの中で描いているため）。UI にブルームなどをかけたくない場合は、フェードと同じく PostEffect の後に描く必要がある。
- テクスチャのスロットは「設定しっぱなし」。読み込み元にしたテクスチャを次に描き込み先にする前に外す（`FullscreenQuad::Draw` は描画後に自動で外す）。

---

## 3. ルール

- 共有する素材はハンドルで持ち、`Get` したポインタはその場で使うだけにする
- 持ち主が1人ではっきりしているもの（RenderTexture、Field の専用メッシュ）は持ち主が直接持つ
- 「テクスチャがあるか」などの判断は、保存せず描く直前に求める
