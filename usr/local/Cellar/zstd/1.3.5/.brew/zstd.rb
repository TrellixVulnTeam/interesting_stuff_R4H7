class Zstd < Formula
  desc "Zstandard is a real-time compression algorithm"
  homepage "http://zstd.net/"
  url "https://github.com/facebook/zstd/archive/v1.3.5.tar.gz"
  sha256 "d6e1559e4cdb7c4226767d4ddc990bff5f9aab77085ff0d0490c828b025e2eea"

  option "without-pzstd", "Build without parallel (de-)compression tool"

  depends_on "cmake" => :build

  def install
    system "make", "install", "PREFIX=#{prefix}/"

    if build.with? "pzstd"
      system "make", "-C", "contrib/pzstd", "googletest"
      system "make", "-C", "contrib/pzstd", "PREFIX=#{prefix}"
      bin.install "contrib/pzstd/pzstd"
    end
  end

  test do
    assert_equal "hello\n",
      pipe_output("#{bin}/zstd | #{bin}/zstd -d", "hello\n", 0)

    if build.with? "pzstd"
      assert_equal "hello\n",
        pipe_output("#{bin}/pzstd | #{bin}/pzstd -d", "hello\n", 0)
    end
  end
end
