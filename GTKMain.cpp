#include <cairomm/context.h>
#include <gtkmm/drawingarea.h>
#include <gtkmm/main.h>
#include <gtkmm/window.h>

#include "Canvas.h"
#include "Juggler.h"
#include "MyException.h"
#include "Siteswap.h"
#include "Utility.h"

namespace
{
	const double slowDown = 2.0;
}

class CairoCanvas : public Canvas
{
	Cairo::RefPtr<Cairo::Context> context;

public:
	CairoCanvas(Cairo::RefPtr<Cairo::Context> c) :
		context(c)
	{
		context->set_source_rgb(0.0, 0.0, 0.0);
		context->paint();

		context->set_source_rgb(1.0, 1.0, 1.0);
	}

	virtual void setColour(double r, double g, double b)
	{
		context->set_source_rgb(r, g, b);
	}

	virtual void moveTo(double x, double y)
	{
		context->move_to(x, y);
	}

	virtual void lineTo(double x, double y)
	{
		context->line_to(x, y);
	}

	virtual void circle(double x, double y, double r)
	{
		context->begin_new_sub_path();
		context->arc(x, y, r, 0, 2 * M_PI);
	}

	virtual void arc(double x, double y, double r, double a, double b)
	{
		context->begin_new_sub_path();
		context->arc(x, y, r, a, b);
	}

	virtual void stroke()
	{
		context->stroke();
	}

	virtual void fill()
	{
		context->fill();
	}
};

class MyDrawingArea : public Gtk::DrawingArea
{
	const Juggler& juggler;

public:
	MyDrawingArea(const Juggler& j) : juggler(j)
	{
		Glib::signal_timeout()
			.connect(sigc::mem_fun(*this, &MyDrawingArea::on_timeout), 30);
	}

protected:
	virtual bool on_expose_event(GdkEventExpose* event)
	{
		Glib::RefPtr<Gdk::Window> window = get_window();
		if (!window)
			return true;

		Gtk::Allocation allocation = get_allocation();
		const int width = allocation.get_width();
		const int height = allocation.get_height();

		Cairo::RefPtr<Cairo::Context> context = window->create_cairo_context();

		if (event)
		{
			context->rectangle(event->area.x, event->area.y,
				event->area.width, event->area.height);
			context->clip();
		}

		{
			double left;
			double right;
			double bottom;
			double top;
			juggler.getBoundingBox(left, right, bottom, top);

			context->translate(width / 2.0, height / 2.0);
			double scale = 0.8
				* std::min(width / (right - left), height / (top - bottom));
			context->scale(scale, -scale);
			context->translate(-(left + right) / 2.0, -(top + bottom) / 2.0);

			context->set_line_width(2.0 / scale);
		}

		CairoCanvas c(context);

		Glib::TimeVal t;
		t.assign_current_time();

		juggler.render(c, t.as_double() / slowDown);

		return true;
	}

	bool on_timeout()
	{
		Glib::RefPtr<Gdk::Window> window = get_window();
		if (!window)
			return true;

		Gtk::Allocation allocation = get_allocation();
		Gdk::Rectangle r(0, 0,
			allocation.get_width(), allocation.get_height());
		window->invalidate_rect(r, false);

		return true;
	}
};

int main(int argc, char** argv)
{
	setProgramName(argv[0]);

	if (argc != 2)
		Usage("usage: ssjuggle <siteswap>");

	try
	{
		SiteswapPattern pattern(argv[1]);
		Juggler juggler(pattern);

		Gtk::Main kit(argc, argv);

		Gtk::Window window;
		window.set_default_size(600, 600);
		window.set_title("Juggler");
		window.set_app_paintable(true);
		window.set_double_buffered(false);

		MyDrawingArea d(juggler);
		window.add(d);
		d.show();

		Gtk::Main::run(window);
	}
	catch (MyException& e)
	{
		Fatal(e.what());
	}

	return 0;
}
